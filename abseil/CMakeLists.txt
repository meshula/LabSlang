cmake_minimum_required(VERSION 3.19)
project(abseil)
add_library(abseil-shim STATIC abseil-shim.cpp)
target_include_directories(abseil-shim PUBLIC ../abseil-cpp)

