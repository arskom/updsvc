#include "Svc.h"
#include <Windows.h>
#include <iostream>

int main(int argc, char *argv[]) {

    std::string domain0, domain1, path0, path1, url, filename;
    domain0 = "ampmail.net";
    path0 = "/release/files.json";
    url = UpdateDetector(CreateRequest(0, domain0, path0));
    urlSplit(url, domain1, path1);
    filename = CreateRequest(1, domain1, path1);
    std::cout << "filename: " << filename << std::endl;

    return 0;
}
