#include "Svc.h"
#include <Msi.h>
#include <Windows.h>
#include <iostream>
#include <shellapi.h>

int main(int argc, char *argv[]) {
    /*std::wstring domain, path;
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

    std::wstring updatepath = L"C:\\Users\\admin\\AppData\\Local\\Temp\\updsvc\\mgui-wgt-4.1.93-"
                              L"x64_mgui-wgt-4.1.70-x64.exe";

    std::wstring t = L"test4.exe";
    std::wstring t1 = L"test2.exe";
    createRegistryEntry(t);
    auto v = GetProgramVersion();
    std ::wcout << v << std::endl;
    auto f = isFilenameBanned(t);
    auto f1 = isFilenameBanned(t1);
    std ::wcout << f << std::endl;
    std ::wcout << f1 << std::endl;

    /*       std::wstring path = L"C:\\Users\\admin\\AppData\\Local\\Temp\\updsvc\\mgui-wgt-4.1.93-"
                               L"x64_mgui-wgt-4.1.70-x64.exe";
   std::size_t lastSlashPos = path.find_last_of(L'\\');

   // Extract the filename from the path
   std::wstring filename = path.substr(lastSlashPos + 1);
   // Output the result
   std::wcout << filename << std::endl;*/
    UpdateifRequires();
    /*auto v = GetProgramVersion();
    std::wcout << v << std::endl;*/
    return 0;
}
