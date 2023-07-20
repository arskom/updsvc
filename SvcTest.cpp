#include "Svc.h"
#include <Windows.h>
#include <iostream>

int main(int argc, char *argv[]) {

    auto source = GetSourcePath();
    auto a = GetFirstFileNameInDirectory(source);
    auto fullpath = source + a;
    const wchar_t *msiPath = fullpath.c_str();
    std::wstring directoryParent, defaultDir;
    if (ReadMSI(msiPath, directoryParent, defaultDir)) {
        // Extraction successful
        wprintf(L"Directory_Parent: %s\n", directoryParent.c_str());
        wprintf(L"DefaultDir: %s\n", defaultDir.c_str());
    }
    else {
        // Reading failed
        std::wcerr << "Reading MSI failed" << std::endl;
    }

    return 0;
}
