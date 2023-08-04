
// SettingsDlg.cpp : implementation file
//

// clang-format off
#include "pch.h"
#include "framework.h"
#include "Settings.h"
#include "SettingsDlg.h"
#include "afxdialogex.h"
// clang-format on

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <vector>
#include <regex>

CString ReadRegistryStringValue(const CString &valueName);
DWORD ReadRegistryDWORDValue(const CString &valueName);
CString GetPathOfWrite();
bool isValidGUID(const CString &str);
std::vector<CString> SetandGetuids(bool uid);

CSettingsDlg::CSettingsDlg(CWnd *pParent /*=nullptr*/)
    : CDialogEx(IDD_SETTINGS_DIALOG, pParent) {
  m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSettingsDlg::DoDataExchange(CDataExchange *pDX) {
  CDialogEx::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_COMBO_RELCHAN, selected_relchan);
  DDX_Control(pDX, IDC_COMBO_PERIOD, selected_period);
  DDX_Control(pDX, IDC_COMBO1, combo_pname);
}

BEGIN_MESSAGE_MAP(CSettingsDlg, CDialogEx)
ON_WM_PAINT()
ON_WM_QUERYDRAGICON()
ON_BN_CLICKED(IDOK, &CSettingsDlg::OnBnClickedOk)
ON_CBN_SELCHANGE(IDC_COMBO_PERIOD, &CSettingsDlg::OnCbnSelchangeComboPeriod)
ON_CBN_SELCHANGE(IDC_COMBO_RELCHAN, &CSettingsDlg::OnCbnSelchangeComboRelchan)
//ON_EN_CHANGE(IDC_EDIT1, &CSettingsDlg::OnEnChangeEdit1)
ON_CBN_SELCHANGE(IDC_COMBO1, &CSettingsDlg::OnCbnSelchangeCombo1)
END_MESSAGE_MAP()

// CSettingsDlg message handlers

BOOL CSettingsDlg::OnInitDialog() {
  CDialogEx::OnInitDialog();

  // Set the icon for this dialog.  The framework does this automatically
  //  when the application's main window is not a dialog
  SetIcon(m_hIcon, TRUE);  // Set big icon
  SetIcon(m_hIcon, FALSE); // Set small icon

  SetWindowLong(this->m_hWnd, GWL_STYLE,
          GetWindowLong(this->m_hWnd, GWL_STYLE) | WS_SYSMENU);
  CWnd *pWnd = GetDlgItem(IDOK);
  pWnd->SendMessage(BCM_SETSHIELD, 0, TRUE);
  //Getting all guids and add name of product to combo box
  std::vector<CString> productnames = SetandGetuids(false);
  for (const CString &productName : productnames) {
      combo_pname.AddString(productName);
  }
  combo_pname.SetCurSel(0);
  //Getting value of combo boxes and setting for starting
  DWORD periodvalueFromRegistry =ReadRegistryDWORDValue(L"PERIOD");
  CString periodcbox;
  if (periodvalueFromRegistry == 3600) {
      periodcbox = L"Every Hour" ;
  }
  else if (periodvalueFromRegistry == 86400) {
      periodcbox = L"Every 24 Hours";      
  }
  else if (periodvalueFromRegistry == 604800) {
      periodcbox = L"Every Week";
  }
  else {
      periodcbox = L"Every 6 Hours";
  }
  // Initialize the ComboBox controls with the value from the registry
  int index1 = selected_period.SelectString(-1, periodcbox);
  if (index1 == CB_ERR) {
      // The value from the registry was not found in the ComboBox1
      return FALSE;
  }
  
  CString channelvalueFromRegistry = ReadRegistryStringValue(L"REL_CHAN");
  if (channelvalueFromRegistry.IsEmpty()) {
      selected_relchan.SelectString(-1, L"Stable");
  }
  // Initialize the ComboBox controls with the value from the registry
  int index2 = selected_relchan.SelectString(-1, channelvalueFromRegistry);
  if (index2 == CB_ERR) {
      // The value from the registry was not found in the ComboBox1
      return FALSE;
  }
  



  return TRUE; // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSettingsDlg::OnPaint() {
  if (IsIconic()) {
    CPaintDC dc(this); // device context for painting

    SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()),
                0);

    // Center icon in client rectangle
    int cxIcon = GetSystemMetrics(SM_CXICON);
    int cyIcon = GetSystemMetrics(SM_CYICON);
    CRect rect;
    GetClientRect(&rect);
    int x = (rect.Width() - cxIcon + 1) / 2;
    int y = (rect.Height() - cyIcon + 1) / 2;

    // Draw the icon
    dc.DrawIcon(x, y, m_hIcon);
  } else {
    CDialogEx::OnPaint();
  }
}

// The system calls this function to obtain the cursor to display while the user
// drags
//  the minimized window.
HCURSOR CSettingsDlg::OnQueryDragIcon() {
  return static_cast<HCURSOR>(m_hIcon);
}

void CSettingsDlg::OnBnClickedOk() {
  CString newExePath = GetPathOfWrite();
  if (newExePath.IsEmpty()) {
    AfxMessageBox(L"Path of executable is empty.");
    return;
  }
  std::vector<CString> uids = SetandGetuids(true);
  int selectedproduct = combo_pname.GetCurSel();
  CString guid = uids[selectedproduct];

  int selectedperiod = selected_period.GetCurSel();
  CString periodstr,paramperiod;
  if (selectedperiod != LB_ERR) {
    selected_period.GetLBText(selectedperiod, periodstr);
  }
  if (periodstr == _T("Every Hour")) {
    paramperiod = L"3600";
  }
  else if (periodstr == _T("Every Hour")) {
    paramperiod = L"21600";
  }
  else if (periodstr == _T("Every 24 Hours")) {
    paramperiod = L"86400";
  }
  else if (periodstr == _T("Every Week")) {
    paramperiod = L"604800";
  }
  else {
    paramperiod = L"0";
  }

  int selectedchannel = selected_relchan.GetCurSel();
  CString relchanstr;
  if (selectedchannel != LB_ERR) {
    selected_relchan.GetLBText(selectedchannel, relchanstr);
  }

  CString param = _T(" ")+guid +  _T(" ")  + paramperiod + _T(" ") + relchanstr;

  HINSTANCE result = ShellExecute(NULL, _T("runas"), newExePath, param, NULL, SW_SHOWNORMAL);

  if ((INT_PTR)result <= 32) {
    DWORD error = GetLastError();
    LPVOID lpMsgBuf;
    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM
                    | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);

    AfxMessageBox((LPCTSTR)lpMsgBuf);

    LocalFree(lpMsgBuf);
  }

  CDialogEx::OnOK();
}

void CSettingsDlg::OnCbnSelchangeComboPeriod() {
  // TODO:Kontrol bildirimi iþleyicinizin kodunu buraya ekleyin
}

void CSettingsDlg::OnCbnSelchangeComboRelchan() {
  // TODO:Kontrol bildirimi iþleyicinizin kodunu buraya ekleyin
}

CString ReadRegistryStringValue( const CString &valueName) {
  HKEY hKey;
  CString result;

  if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Arskom\\updsvc", 0, KEY_READ, &hKey)
          == ERROR_SUCCESS) {
    TCHAR buffer[MAX_PATH];
    DWORD bufferSize = sizeof(buffer);

    if (RegQueryValueEx(
                hKey, valueName, nullptr, nullptr, reinterpret_cast<LPBYTE>(buffer), &bufferSize)
            == ERROR_SUCCESS) {
        result = buffer;
    }

    RegCloseKey(hKey);
  }

  return result;
}

DWORD ReadRegistryDWORDValue(const CString &valueName) {
  DWORD data = 0;
  CRegKey regKey;

  // Open the registry key (adjust the key name as needed)
  LONG result = regKey.Open(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Arskom\\updsvc", KEY_READ);
  if (result == ERROR_SUCCESS) {
    // Read the DWORD value from the registry
    result = regKey.QueryDWORDValue(valueName, data);
    regKey.Close();
  }

  if (result != ERROR_SUCCESS) {
    // Handle the error if the key cannot be opened or the value cannot be read
    return 0; // Return a default value or an appropriate error code
  }

  return data;
}

CString GetPathOfWrite() {
  TCHAR exePath[MAX_PATH];
  DWORD pathLength = GetModuleFileName(nullptr, exePath, MAX_PATH);
  if (pathLength == 0 || pathLength >= MAX_PATH) {
    // Handle the error if GetModuleFileName fails
    AfxMessageBox(L"Failed to get the path of the current executable.");
    return {};
  }

  // Remove the filename part to get the directory path
  PathRemoveFileSpec(exePath);

  CString newExePath;
  newExePath.Format(_T("%s\\updsvc-write.exe"), exePath);
  return newExePath;
}

void CSettingsDlg::OnEnChangeEdit1() {
  // TODO:  If this is a RICHEDIT control, the control will not
  // send this notification unless you override the CDialogEx::OnInitDialog()
  // function and call CRichEditCtrl().SetEventMask()
  // with the ENM_CHANGE flag ORed into the mask.

  // TODO:  Add your control notification handler code here
}

void CSettingsDlg::OnCbnSelchangeCombo1() {

}

std::vector<CString> SetandGetuids(bool uid) {
  HKEY hKey;  
  if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Arskom\\updsvc", 0, KEY_READ, &hKey)
          != ERROR_SUCCESS) {
   // SvcReportEvent(L"Unable to open registry key: SOFTWARE\\Arskom\\updsvc");
    return {};
  }
  std::vector<CString> str;
  wchar_t guid[MAX_PATH];
  DWORD index = 0;
  while (RegEnumKey(hKey, index, guid, MAX_PATH) == ERROR_SUCCESS) {
    CString product_guid(guid);

    if (isValidGUID(product_guid)) {
        if (uid) {
            str.push_back(product_guid);
        }
        else {
            HKEY hSubKey;
            if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
                        L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\" + product_guid,
                        0, KEY_READ, &hSubKey)
                    == ERROR_SUCCESS) {
                wchar_t displayName[MAX_PATH];
                DWORD displayNameSize = sizeof(displayName);
                if (RegQueryValueEx(hSubKey, L"DisplayName", NULL, NULL,
                            reinterpret_cast<LPBYTE>(displayName), &displayNameSize)
                        == ERROR_SUCCESS) {
                    str.push_back(displayName);
                    RegCloseKey(hSubKey);
                }
            }
        }
    }
    index++;
  }
  RegCloseKey(hKey);
  return str;
}

bool isValidGUID(const CString &str) {
  const std::wregex uidPattern(L"(\\{[0-9A-Fa-f]{8}-[0-9A-Fa-f]{4}-[0-9A-Fa-f]{4}-[0-9A-Fa-f]{4}-[0-9A-Fa-f]{12}\\})");

  return std::regex_match(str.GetString(), uidPattern);
}