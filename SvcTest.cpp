#include "Svc.h"
#include <iostream>

int main(int argc, char *argv[]) {
    /*CreateRequest();
    std::string version = GetProgramVersion();
    if (!version.empty())
    {
        std::cout << "Program Version: " << version << std::endl;
    }*/

    // UpdateDetector(CreateRequest());
    std::string domain, path, url;
    url = "https://ampmail.net/release/mgui-wgt/mgui-wgt-4.1.93-x64_mgui-wgt-4.1.92-x64.exe";
    domain = urlSplit(0, url);
    path = urlSplit(1, url);
    std::cout << "domain:" << domain << "    path:" << path << std::endl;
    return 0;
}
