# https://docs.godotengine.org/en/latest/tutorials/scripting/cpp/build_system/cmake.html
cmake_minimum_required(VERSION 3.20)

if(CMAKE_HOST_APPLE AND NOT DEFINED CMAKE_OSX_ARCHITECTURES)
    set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64" CACHE STRING "")
endif()

project(serial_scanner LANGUAGES C CXX)

set(SERIAL_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../..)
set(ADDON_BIN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/addons/serial_scanner/bin)

set(GODOTCPP_DISABLE_EXCEPTIONS OFF CACHE BOOL "" FORCE)

set(GODOTCPP_BUILD_PROFILE ${CMAKE_CURRENT_SOURCE_DIR}/build_profile.json CACHE PATH "")

add_subdirectory(godot-cpp)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_subdirectory(${SERIAL_ROOT}/third-party/serial_cpp ${CMAKE_CURRENT_BINARY_DIR}/serial_cpp EXCLUDE_FROM_ALL)

add_library(serial_scanner SHARED
    src/register_types.cpp
    src/register_types.h
    src/serial_scanner.cpp
    src/serial_scanner.h
    ${SERIAL_ROOT}/src/Scanner.cpp
)

target_compile_features(serial_scanner PRIVATE cxx_std_20)

target_include_directories(serial_scanner PRIVATE
    ${SERIAL_ROOT}/include
)

target_link_libraries(serial_scanner PRIVATE
    godot::cpp
    serial_cpp
)

get_target_property(GODOTCPP_SUFFIX godot::cpp GODOTCPP_SUFFIX)

set_target_properties(serial_scanner PROPERTIES
    PREFIX ""
    OUTPUT_NAME "serial_scanner${GODOTCPP_SUFFIX}"
    LIBRARY_OUTPUT_DIRECTORY "$<1:${ADDON_BIN_DIR}>"
    RUNTIME_OUTPUT_DIRECTORY "$<1:${ADDON_BIN_DIR}>"
)
