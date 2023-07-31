#ifndef SVC_H
#define SVC_H

#include <Windows.h>
#include <sstream>

struct UpdateInfo {
    std::wstring url;
    bool is_patch = false;
};

struct Config {
    std::wstring url;
    std::wstring product_guid;
    std::wstring params_full;
    std::wstring params_patch;
    DWORD period;
};

std::string CreateRequest(bool file, const std::wstring &domain, const std::wstring &path);
std::wstring GetProgramVersion(Config cfg);
UpdateInfo UpdateDetector(const std::string &sstr);
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
std::wstring ReadMSI(Config cfg, const wchar_t *msiPath);
std::wstring GetFirstFileNameInDirectory(const std::wstring &directoryPath);
std::wstring GetMSIProperty(const std::wstring &msiFilePath, const std::wstring &propertyName);
int isRunning();
void Update();
void createRegistryEntry(std::wstring path, std::wstring filename);
bool isFilenameBanned(const std::wstring &filename);
std::wstring UpdateifRequires(Config cfg);
std::wstring readDataString(std::wstring keyPath, std::wstring valueName);
DWORD ReadDWORDFromRegedit(std::wstring keyPath, std::wstring regValueName);
#endif // SVC_H
