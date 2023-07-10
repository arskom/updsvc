project(updsvc)

cmake_minimum_required(VERSION 3.15)

add_custom_command(
    OUTPUT UpdSvc.rc
    MAIN_DEPENDENCY UpdSvc.mc
    COMMAND mc -U ${CMAKE_SOURCE_DIR}/UpdSvc.mc
)

add_custom_command(
    OUTPUT UpdSvc.res
    MAIN_DEPENDENCY UpdSvc.rc
    COMMAND rc -r UpdSvc.rc
)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
add_executable(updsvc     UpdSvc.res Svc.cpp)
add_executable(updsvc-ctl UpdSvc.res SvcControl.cpp)
add_executable(updsvc-cfg UpdSvc.res SvcConfig.cpp)
