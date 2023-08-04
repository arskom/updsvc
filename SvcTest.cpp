#include "Svc.h"
#include <Msi.h>
#include <Windows.h>
#include <iostream>
#include <shellapi.h>

int main(int argc, char *argv[]) {
    DWORD period = 0;
    UpdateAll(period);
    return 0;
}
