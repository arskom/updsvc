#include "Svc.h"
#include <Msi.h>
#include <Windows.h>
#include <iostream>
#include <shellapi.h>

int main(int argc, char *argv[]) {
    Config cfg;
    cfg.product_guid = readDataString(L"SOFTWARE\\Arskom\\updsvc", L"PRODUCT_GUID");
    cfg.url = readDataString(L"SOFTWARE\\Arskom\\updsvc\\" + cfg.product_guid, L"URL");
    cfg.params_full =
            readDataString(L"SOFTWARE\\Arskom\\updsvc\\" + cfg.product_guid, L"PARAMS_FULL");
    cfg.params_patch =
            readDataString(L"SOFTWARE\\Arskom\\updsvc\\" + cfg.product_guid, L"PARAMS_PATCH");
    cfg.rel_chan = readDataString(L"SOFTWARE\\Arskom\\updsvc\\" + cfg.product_guid, L"REL_CHAN");
    cfg.period = ReadDWORDFromRegedit(L"SOFTWARE\\Arskom\\updsvc\\" + cfg.product_guid, L"PERIOD");

    if (cfg.period == 0) {
        return -1;
    }
    if (cfg.url.empty()) {
        SvcReportEvent(L"Service cant start, url is empty or getting url from registry");
        return -1;
    }
    if (cfg.product_guid.empty()) {
        SvcReportEvent(L"Service cant start, product guid is empty or getting it from registry");
        return -1;
    }
    if (cfg.params_full.empty()) {
        SvcReportEvent(
                L"Service cant start, full install parameter is empty or getting it from registry");
        return -1;
    }
    if (cfg.params_patch.empty()) {
        SvcReportEvent(L"Service cant start, patch install parameter is empty or getting it from "
                       L"registry");
        return -1;
    }
    UpdateifRequires(cfg);
    return 0;
}
