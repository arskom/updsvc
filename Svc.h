#ifndef SVC_H
#define SVC_H

#include "json.hpp"
#include <sstream>

struct Buffer {
    char *data;
    unsigned long size;

    Buffer() : data(nullptr), size(0) {}
    Buffer(char *d, unsigned long s) : data(d), size(s) {}

    ~Buffer() {
        if (data) {
            delete[] data;
        }
    }
};

std::string CreateRequest();
std::string GetProgramVersion();
std::string UpdateDetector(std::string sstr);
int compareVersions(const std::string &version1, const std::string &version2);

#endif // SVC_H
