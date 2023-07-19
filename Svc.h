#ifndef SVC_H
#define SVC_H

#include "json.hpp"
#include <Windows.h>
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

std::string CreateRequest(bool file, std::string &domain, std::string &path);
std::string GetProgramVersion();
std::string UpdateDetector(std::string sstr);
int compareVersions(const std::string &version1, const std::string &version2);
void urlSplit(const std::string &url, std::string &domain, std::string &path);
bool checkandCreateDirectory(std::string path);
std::wstring s2ws(const std::string &s);

#endif // SVC_H
