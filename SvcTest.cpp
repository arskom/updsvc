#include "Svc.h"
#include <iostream>

int main(int argc, char *argv[]) {
    /*CreateRequest();
    std::string version = GetProgramVersion();
    if (!version.empty())
    {
        std::cout << "Program Version: " << version << std::endl;
    }

    // UpdateDetector(CreateRequest());
    std::string url =
            "https://ampmail.net/release/mgui-wgt/mgui-wgt-4.1.93-x64_mgui-wgt-4.1.92-x64.exe";

    std::string domain, path, filename;
    urlSplit(url, domain, path, filename);

    std::cout << "Domain: " << domain << std::endl;
    std::cout << "Path: " << path << std::endl;
    std::cout << "Filename: " << filename << std::endl;*/

    const char *tempDir = std::getenv("TEMP");
    if (tempDir == nullptr) {
        std::cerr << "Failed to retrieve the temporary directory path." << std::endl;
    }
    std::string filename = "mgui-wgt-4.1.93-x64_mgui-wgt-4.1.92-x64.exe";
    std::string tempFilePath = std::string(tempDir) + "\\" + filename;
    std::cout << "file path: " << tempFilePath << std::endl;

    return 0;
}
