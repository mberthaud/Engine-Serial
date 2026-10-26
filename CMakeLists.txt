cmake_minimum_required(VERSION 3.20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

project(MySerial LANGUAGES C CXX)

#disable installs for serial-cpp
set(serial_cpp_INSTALL OFF)

# Dependencies
add_subdirectory(third-party/serial_cpp)

# Library
add_library(MySerial SHARED
    src/Scanner.cpp
    src/ScannerC.cpp
    include/ScannerC.hpp
    include/Scanner.hpp)

target_compile_features(MySerial PUBLIC cxx_std_20)


target_include_directories(MySerial PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(MySerial PUBLIC
    serial_cpp
)

# example executable to test stuff
add_executable(Test
    test/test.cpp
)

target_link_libraries(Test PRIVATE
    MySerial
)

set_target_properties(MySerial Test PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)
