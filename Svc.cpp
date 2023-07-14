
#include <windows.h>

#include <strsafe.h>
#include <tchar.h>

#include <winhttp.h>

#include "UpdSvc.h"
#include "Svc.h"
#include "json.hpp"

#include <sstream>
#include <iostream>

#include <Msi.h>

#define uid TEXT("{028818E2-5DF4-414F-A1E4-2AA542DE4697}")

#define SVCNAME TEXT("UpdSvc")
static SERVICE_STATUS gSvcStatus;
static SERVICE_STATUS_HANDLE gSvcStatusHandle;
static HANDLE ghSvcStopEvent = NULL;

VOID SvcInstall(void);
VOID WINAPI SvcCtrlHandler( DWORD ); 
VOID WINAPI SvcMain( DWORD, LPTSTR * ); 

VOID ReportSvcStatus( DWORD, DWORD, DWORD );
VOID SvcInit(DWORD, LPTSTR *);
VOID SvcReportEvent(LPTSTR);

/**
 * @brief Entry point for the process
 * @param argc Number of arguments
 * @param argv Argument contents
 * @return None, defaults to 0
 */

#ifndef SVC_TEST

int __cdecl _tmain(int argc, TCHAR *argv[])
{
    // If command-line parameter is "install", install the service.
    // Otherwise, the service is probably being started by the SCM.

    if( lstrcmpi( argv[1], TEXT("install")) == 0 )
    {
        SvcInstall();
        return 0;
    }

    // TO_DO: Add any additional services for the process to this table.
    SERVICE_TABLE_ENTRY DispatchTable[] = 
    { 
        { SVCNAME, (LPSERVICE_MAIN_FUNCTION) SvcMain }, 
        { NULL, NULL } 
    }; 
 
    // This call returns when the service has stopped. 
    // The process should simply terminate when the call returns.

    if (!StartServiceCtrlDispatcher(DispatchTable)) {
        SvcReportEvent(TEXT("StartServiceCtrlDispatcher"));
    }

    return 0;
}

#endif

//
// Purpose:
//   Installs a service in the SCM database
//
// Parameters:
//   None
//
// Return value:
//   None
//
VOID SvcInstall()
{
    SC_HANDLE schSCManager;
    SC_HANDLE schService;
    TCHAR szUnquotedPath[MAX_PATH];

    if( !GetModuleFileName( NULL, szUnquotedPath, MAX_PATH ) )
    {
        printf("Cannot install service (%d)\n", GetLastError());
        return;
    }

    // In case the path contains a space, it must be quoted so that
    // it is correctly interpreted. For example,
    // "d:\my share\myservice.exe" should be specified as
    // ""d:\my share\myservice.exe"".
    TCHAR szPath[MAX_PATH];
    StringCbPrintf(szPath, MAX_PATH, TEXT("\"%s\""), szUnquotedPath);

    // Get a handle to the SCM database. 
 
    schSCManager = OpenSCManager( 
        NULL,                    // local computer
        NULL,                    // ServicesActive database 
        SC_MANAGER_ALL_ACCESS);  // full access rights 
 
    if (NULL == schSCManager) 
    {
        printf("OpenSCManager failed (%d)\n", GetLastError());
        return;
    }

    // Create the service

    schService = CreateService( 
        schSCManager,              // SCM database 
        SVCNAME,                   // name of service 
        SVCNAME,                   // service name to display 
        SERVICE_ALL_ACCESS,        // desired access 
        SERVICE_WIN32_OWN_PROCESS, // service type 
        SERVICE_DEMAND_START,      // start type 
        SERVICE_ERROR_NORMAL,      // error control type 
        szPath,                    // path to service's binary 
        NULL,                      // no load ordering group 
        NULL,                      // no tag identifier 
        NULL,                      // no dependencies 
        NULL,                      // LocalSystem account 
        NULL);                     // no password 
 
    if (schService == NULL) 
    {
        printf("CreateService failed (%d)\n", GetLastError()); 
        CloseServiceHandle(schSCManager);
        return;
    }
    else printf("Service installed successfully\n"); 

    CloseServiceHandle(schService); 
    CloseServiceHandle(schSCManager);
}

//
// Purpose: 
//   Entry point for the service
//
// Parameters:
//   dwArgc - Number of arguments in the lpszArgv array
//   lpszArgv - Array of strings. The first string is the name of
//     the service and subsequent strings are passed by the process
//     that called the StartService function to start the service.
// 
// Return value:
//   None.
//
VOID WINAPI SvcMain( DWORD dwArgc, LPTSTR *lpszArgv )
{
    // Register the handler function for the service

    gSvcStatusHandle = RegisterServiceCtrlHandler( 
        SVCNAME, 
        SvcCtrlHandler);

    if( !gSvcStatusHandle )
    { 
        SvcReportEvent(TEXT("RegisterServiceCtrlHandler")); 
        return; 
    } 

    // These SERVICE_STATUS members remain as set here

    gSvcStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS; 
    gSvcStatus.dwServiceSpecificExitCode = 0;    

    // Report initial status to the SCM

    ReportSvcStatus( SERVICE_START_PENDING, NO_ERROR, 3000 );

    // Perform service-specific initialization and work.

    SvcInit( dwArgc, lpszArgv );
}

//
// Purpose: 
//   The service code
//
// Parameters:
//   dwArgc - Number of arguments in the lpszArgv array
//   lpszArgv - Array of strings. The first string is the name of
//     the service and subsequent strings are passed by the process
//     that called the StartService function to start the service.
// 
// Return value:
//   None
//
VOID SvcInit( DWORD dwArgc, LPTSTR *lpszArgv)
{
    // TO_DO: Declare and set any required variables.
    //   Be sure to periodically call ReportSvcStatus() with 
    //   SERVICE_START_PENDING. If initialization fails, call
    //   ReportSvcStatus with SERVICE_STOPPED.

    // Create an event. The control handler function, SvcCtrlHandler,
    // signals this event when it receives the stop control code.

    ghSvcStopEvent = CreateEvent(
                         NULL,    // default security attributes
                         TRUE,    // manual reset event
                         FALSE,   // not signaled
                         NULL);   // no name

    if ( ghSvcStopEvent == NULL)
    {
        ReportSvcStatus( SERVICE_STOPPED, GetLastError(), 0 );
        return;
    }

    // Report running status when initialization is complete.

    ReportSvcStatus( SERVICE_RUNNING, NO_ERROR, 0 );

    // TO_DO: Perform work until service stops.

    while(1)
    {
        // Check whether to stop the service.

        WaitForSingleObject(ghSvcStopEvent, INFINITE);

        ReportSvcStatus( SERVICE_STOPPED, NO_ERROR, 0 );

        CreateRequest();

        return;
    }
}

//
// Purpose: 
//   Sets the current service status and reports it to the SCM.
//
// Parameters:
//   dwCurrentState - The current state (see SERVICE_STATUS)
//   dwWin32ExitCode - The system error code
//   dwWaitHint - Estimated time for pending operation, 
//     in milliseconds
// 
// Return value:
//   None
//
VOID ReportSvcStatus( DWORD dwCurrentState,
                      DWORD dwWin32ExitCode,
                      DWORD dwWaitHint)
{
    static DWORD dwCheckPoint = 1;

    // Fill in the SERVICE_STATUS structure.

    gSvcStatus.dwCurrentState = dwCurrentState;
    gSvcStatus.dwWin32ExitCode = dwWin32ExitCode;
    gSvcStatus.dwWaitHint = dwWaitHint;

    if (dwCurrentState == SERVICE_START_PENDING)
        gSvcStatus.dwControlsAccepted = 0;
    else gSvcStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;

    if ( (dwCurrentState == SERVICE_RUNNING) ||
           (dwCurrentState == SERVICE_STOPPED) )
        gSvcStatus.dwCheckPoint = 0;
    else gSvcStatus.dwCheckPoint = dwCheckPoint++;

    // Report the status of the service to the SCM.
    SetServiceStatus( gSvcStatusHandle, &gSvcStatus );
}

//
// Purpose: 
//   Called by SCM whenever a control code is sent to the service
//   using the ControlService function.
//
// Parameters:
//   dwCtrl - control code
// 
// Return value:
//   None
//
VOID WINAPI SvcCtrlHandler( DWORD dwCtrl )
{
   // Handle the requested control code. 

   switch(dwCtrl) 
   {  
      case SERVICE_CONTROL_STOP: 
         ReportSvcStatus(SERVICE_STOP_PENDING, NO_ERROR, 0);

         // Signal the service to stop.

         SetEvent(ghSvcStopEvent);
         ReportSvcStatus(gSvcStatus.dwCurrentState, NO_ERROR, 0);
         
         return;
 
      case SERVICE_CONTROL_INTERROGATE: 
         break; 
 
      default: 
         break;
   } 
   
}

//
// Purpose: 
//   Logs messages to the event log
//
// Parameters:
//   szFunction - name of function that failed
// 
// Return value:
//   None
//
// Remarks:
//   The service must have an entry in the Application event log.
//
VOID SvcReportEvent(LPTSTR szFunction) 
{ 
    HANDLE hEventSource;
    LPCTSTR lpszStrings[2];
    TCHAR Buffer[80];

    hEventSource = RegisterEventSource(NULL, SVCNAME);

    if( NULL != hEventSource )
    {
        StringCchPrintf(Buffer, 80, TEXT("%s failed with %d"), szFunction, GetLastError());

        lpszStrings[0] = SVCNAME;
        lpszStrings[1] = Buffer;

        ReportEvent(hEventSource,        // event log handle
                    EVENTLOG_ERROR_TYPE, // event type
                    0,                   // event category
                    SVC_ERROR,           // event identifier
                    NULL,                // no security identifier
                    2,                   // size of lpszStrings array
                    0,                   // no binary data
                    lpszStrings,         // array of strings
                    NULL);               // no binary data

        DeregisterEventSource(hEventSource);
    }
}

std::string CreateRequest()
{
    DWORD dwSize=0;
    DWORD dwDownloaded=0;
    LPSTR pszOutBuffer;

    std::stringstream sstr;

    BOOL  bResults = FALSE;
    HINTERNET hSession = NULL,
        hConnect = NULL,
        hRequest = NULL;

    // Use WinHttpOpen to obtain a session handle.
    hSession = WinHttpOpen(  L"Http Request Attempt",
                           WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
                           WINHTTP_NO_PROXY_NAME,
                           WINHTTP_NO_PROXY_BYPASS, 0);

    // Specify an HTTP server.
    if (hSession){
        hConnect = WinHttpConnect( hSession, L"ampmail.net",
                                  INTERNET_DEFAULT_PORT, 0);

    }
    // Create an HTTP Request handle.
    if (hConnect){
        hRequest = WinHttpOpenRequest( hConnect, L"GET",
                                      L"/release/files.json",
                                      NULL, WINHTTP_NO_REFERER,
                                      WINHTTP_DEFAULT_ACCEPT_TYPES,
                                      0);
    }
    // Send a Request.
    if (hRequest){
        bResults = WinHttpSendRequest( hRequest,
                                      WINHTTP_NO_ADDITIONAL_HEADERS,
                                      0, WINHTTP_NO_REQUEST_DATA, 0,
                                      0, 0);
        SvcReportEvent("request sent");
    }

    // End the request.
    if (bResults)
        bResults = WinHttpReceiveResponse( hRequest, NULL);

    // Keep checking for data until there is nothing left.
    if (bResults)
    {
        do
        {
            // Check for available data.
            dwSize = 0;
            if (!WinHttpQueryDataAvailable( hRequest, &dwSize))
            {
                printf( "Error %u in WinHttpQueryDataAvailable.\n",
                       GetLastError());
                break;
            }

            // No more available data.
            if (!dwSize)
                break;

            // Allocate space for the buffer.
            pszOutBuffer = new char[dwSize+1];
            if (!pszOutBuffer)
            {
                printf("Out of memory\n");
                break;
            }

            // Read the Data.
            ZeroMemory(pszOutBuffer, dwSize+1);

            if (!WinHttpReadData( hRequest, (LPVOID)pszOutBuffer,
                                 dwSize, &dwDownloaded))
            {
                printf( "Error %u in WinHttpReadData.\n", GetLastError());
                SvcReportEvent("failed to read");
                break;
            }

            assert(dwSize == dwDownloaded);
            sstr << std::string_view(pszOutBuffer,dwSize);
            delete[] pszOutBuffer;
            SvcReportEvent("data read");

            // This condition should never be reached since WinHttpQueryDataAvailable
            // reported that there are bits to read.
            if (dwDownloaded == 0)
                break;

        } while (dwSize > 0);
    }
    else
    {
        // Report any errors.
        printf( "Error %d has occurred.\n", GetLastError() );
    }

    // Close any open handles.
    if (hRequest) WinHttpCloseHandle(hRequest);
    if (hConnect) WinHttpCloseHandle(hConnect);
    if (hSession) WinHttpCloseHandle(hSession);
    return sstr.str();
}

void Parse(std::string sstr){
    using json = nlohmann::json;
    json j_complete;
    try
    {
        // parsing input with a syntax error
        j_complete = json::parse(sstr);

    }
    catch (json::parse_error& e)
    {
        // output exception information
        std::cout << "message: " << e.what() << '\n'
                  << "exception id: " << e.id << '\n'
                  << "byte position of error: " << e.byte << std::endl;
        return;
    }

    /*std::string s = j_complete["mgui-wgt"]["exe"]["4.1.92"]["url"];
    std::cout << "exe: " << s << std::endl;*/
}

//Function to retrieve the version of a program
std::string GetProgramVersion()
{
    char versionBuffer[256];
    DWORD bufferSize = sizeof(versionBuffer);

    //Use MsiGetProductInfo for get the version
    UINT result = MsiGetProductInfo(uid, INSTALLPROPERTY_VERSIONSTRING, versionBuffer, &bufferSize);
    if (result == ERROR_SUCCESS)
    {
        return std::string(versionBuffer);
    }

    return "";
}

// Function to initiate an update for a program using the unique identifier (GUID)
/*bool InitiateProgramUpdat(const std::string& updatePackage)
{
    UINT result = MsiConfigureProduct(uid, INSTALLLEVEL_DEFAULT, INSTALLSTATE_DEFAULT);
    return (result == ERROR_SUCCESS);
}*/

/*bool Update(const std::string& updatePackage)
{

}*/
