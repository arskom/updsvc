#include "Svc.h"
#include <Windows.h>
#include <iostream>

int main(int argc, char *argv[]) {

    std::wstring domain0, domain1, path0, path1, url;
    domain0 = L"ampmail.net";
    path0 = L"/release/files.json";
    auto data = CreateRequest(0, domain0, path0);
    url = UpdateDetector(data);
    urlSplit(url, domain1, path1);
    auto filename = CreateRequest(1, domain1, path1);
    std::cout << "filename: " << filename << std::endl;
    auto version = GetProgramVersion();
    std::wcout << "version: " << version << std::endl;

    auto path = GetProgramVersion();
    std::wcout << "path: " << path << std::endl;
    /*std::wstring url =
            L"https://ampmail.net/release/mgui-wgt/mgui-wgt-4.1.93-x64_mgui-wgt-4.1.92-x64.exe";
    std::wstring domain, path;
    urlSplit(url, domain, path);
    std::wcout << "domain: " << domain << std::endl;
    std::wcout << "path: " << path << std::endl;
    std::size_t lastSlashPos = path.find_last_of(L"/");
    auto filename = path.substr(lastSlashPos + 1);
    std::wcout << "filename: " << filename << std::endl;*/
    return 0;
}
