#ifndef SVC_H
#define SVC_H

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

std::string CreateRequest(bool file, const std::wstring &domain, const std::wstring &path);
std::wstring GetProgramVersion();
std::wstring UpdateDetector(const std::string &sstr);
int compareVersions(const std::string &version1, const std::wstring &version2);
void urlSplit(const std::wstring &url, std::wstring &domain, std::wstring &path);
bool checkandCreateDirectory(std::wstring path);
std::wstring s2ws(std::string_view s);
inline std::wstring s2ws(const std::string &s) {
    return s2ws(std::string_view{s});
}
std::string ws2s(std::wstring_view s);
inline std::string ws2s(const std::wstring &s) {
    return ws2s(std::wstring_view{s});
}
std::wstring GetSourcePath();
bool ReadMSI(const wchar_t *msiPath, std::wstring &dirparent, std::wstring &defaultdir);
std::wstring GetFirstFileNameInDirectory(const std::wstring &directoryPath);
std::wstring GetMSIProperty(const std::wstring &msiFilePath, const std::wstring &propertyName);
int isRunning();
void Update();

#endif // SVC_H
