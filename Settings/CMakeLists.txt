cmake_minimum_required(VERSION 3.19)
project(Settings)

set(CMAKE_MFC_FLAG 2)
set(CMAKE_CXX_STANDARD 17)
add_compile_definitions(UNICODE _UNICODE)

add_executable(updsvc-settings
    framework.h
    targetver.h
    Settings.rc
    Settings.h
    Settings.cpp
    SettingsDlg.h
    SettingsDlg.cpp
)

target_compile_definitions(updsvc-settings PRIVATE _AFXDLL)
target_link_options(updsvc-settings PRIVATE "/entry:wWinMainCRTStartup")
set_target_properties(updsvc-settings PROPERTIES WIN32_EXECUTABLE TRUE)

add_executable(updsvc-write
    Write.cpp
)

set_target_properties(updsvc-write PROPERTIES WIN32_EXECUTABLE TRUE)

