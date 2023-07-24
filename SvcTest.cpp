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
    std::wstring domain1, path1;
    urlSplit(updateurl, domain1, path1);
    CreateRequest(1, domain1, path1);

    return 0;
}
