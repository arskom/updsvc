#include "Svc.h"
#include <Windows.h>
#include <iostream>

int main(int argc, char *argv[]) {

    const wchar_t *msiPath =
            L"C:\\ProgramData\\Package "
            L"Cache\\{028818E2-5DF4-414F-A1E4-2AA542DE4697}v4.1.92\\mgui-wgt-4-x64.msi";
    std::wstring directoryParent, defaultDir;
    if (ReadMSI(msiPath, directoryParent, defaultDir)) {
        // Extraction successful
        // Now you can use directoryParent and defaultDir as needed
        // For example, you can print their values:
        wprintf(L"Directory_Parent: %s\n", directoryParent.c_str());
        wprintf(L"DefaultDir: %s\n", defaultDir.c_str());
    }
    else {
        // Reading failed
        std::wcerr << "Reading MSI failed" << std::endl;
    }

    return 0;
}
