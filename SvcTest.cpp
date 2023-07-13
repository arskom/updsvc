#include "Svc.h"
#include <iostream>

int main(int argc, char *argv[])
{
    //CreateRequest();
    std::string version = GetProgramVersion();
    if (!version.empty())
    {
        std::cout << "Program Version: " << version << std::endl;
    }
    Parse(CreateRequest());
    return 0;
}

