cmake_minimum_required(VERSION 3.19)
project(LabSlang)
set(LABSLANG_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

if (POLICY CMP0077)
    cmake_policy(SET CMP0077 NEW)
endif()

# glfw is required to get a bunch of the helper functionality
# if the platform bindings can be made available without glfw
# this can be removed.
find_package(glfw3 REQUIRED)

#------------------------------------------------------------------------------
# Build LabSlang, this will also build Tint and spirv-tools

option(LABSLANG_BUILD_DAWN "" ON)
set(LABSLANG_DAWN_INSTALL_ROOT "C:/tmp/install" CACHE PATH "Location of installed Dawn")
if(LABSLANG_BUILD_DAWN)
    set(glfw_FOUND ON)
    option(DAWN_BUILD_EXAMPLES "" OFF)
    include(dawn.cmake)
endif()

#set(IMGUI_BACKEND_WGPU ON)
#set(IMGUI_BACKEND_GLFW ON)
#set(IMGUI_BACKEND_DAWN ON)
#set(WEBGPU_HEADER_LOCATION "${CMAKE_INSTALL_PREFIX}/include")
#include(imgui.cmake)

set(LABSLANG_DAWN_LIBS
    dawncpp
    dawn_common
    dawn_internal_config
    dawn_native
    dawn_proc
    dawn_utils
    dawn_wire)

