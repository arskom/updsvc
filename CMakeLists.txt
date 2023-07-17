project(updsvc)

cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

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

# updsvc
add_executable(updsvc     UpdSvc.res Svc.cpp)
target_link_libraries(updsvc PRIVATE Winhttp advapi32 msi)

# updsvc_test
add_executable(updsvc_test     Svc.cpp Svc.h SvcTest.cpp)
target_link_libraries(updsvc_test PRIVATE Winhttp advapi32 msi)
target_compile_definitions(updsvc_test PRIVATE SVC_TEST)

add_executable(updsvc-ctl UpdSvc.res SvcControl.cpp)
add_executable(updsvc-cfg UpdSvc.res SvcConfig.cpp)
