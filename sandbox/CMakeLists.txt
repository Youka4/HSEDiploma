cmake_minimum_required(VERSION 3.23)
project(untitled)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include_directories(SYSTEM "/Library/Developer/CommandLineTools/SDKs/MacOSX15.4.sdk/usr/include/c++/v1")
include_directories(SYSTEM "/Library/Developer/CommandLineTools/SDKs/MacOSX15.4.sdk/usr/include")

set(CMAKE_OSX_SYSROOT "/Library/Developer/CommandLineTools/SDKs/MacOSX15.4.sdk")



set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

add_executable(untitled version_1.0.cpp)
add_executable(SK x5.cpp)
add_executable(untitled2 main2.cpp)