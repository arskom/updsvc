#include "Svc.h"
#include <Msi.h>
#include <Windows.h>
#include <iostream>
#include <shellapi.h>

int main(int argc, char *argv[]) {
    std::wstring domain, path;
    domain = L"ampmail.net";
    path = L"/release/files.json";
    std::string json = CreateRequest(0, domain, path);
    auto updateurl = UpdateDetector(json);
    // error handling if url exists
    std::wstring domain1, path1;
    urlSplit(updateurl, domain1, path1);
    auto a = CreateRequest(
            1, domain1, path1); // this function has to be return path of downloaded file
    std ::cout << "This text is returned from create request: " << a << std::endl;
    /*std::wstring updatepath = L"C:\\Users\\admin\\AppData\\Local\\Temp\\updsvc\\mgui-wgt-4.1.93-"
                              L"x64_mgui-wgt-4.1.70-x64.exe";
    HINSTANCE hInst = ShellExecute(NULL, L"runas", updatepath.c_str(), NULL, NULL, SW_SHOWNORMAL);*/

    // then call update function to make update

    /*if ((int)hInst > 32) {
        std::wcout << "Installed succesfully" << std ::endl;
    }
    else {
        // ShellExecute failed
        DWORD errorCode = (DWORD)hInst;
        printf("Installation failed (%lu)\n", GetLastError());
        std::wcout << errorCode << std ::endl;
    }*/

    auto v = GetProgramVersion();
    std ::wcout << v << std::endl;
    return 0;
}
