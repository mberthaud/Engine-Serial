cmake_minimum_required(VERSION 3.20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

project(MySerial LANGUAGES C CXX)

#disable installs for serial-cpp
set(serial_cpp_INSTALL OFF)

# Dependencies
add_subdirectory(third-party/serial_cpp)

# Library
add_library(MySerial STATIC
    src/Scanner.cpp
    bindings/c/scanner_c.c
    include/Scanner.hpp)

target_compile_features(MySerial PUBLIC cxx_std_20)


target_include_directories(MySerial PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/
)

target_link_libraries(MySerial PUBLIC
    serial_cpp
)

# example executable to test stuff
add_executable(Example
    example/example.cpp
)

target_link_libraries(Example PRIVATE
    MySerial
)

set_target_properties(MySerial Example PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)
