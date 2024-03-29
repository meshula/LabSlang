cmake_minimum_required(VERSION 3.19)
project(LabSlang)
set(LABSLANG_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# policy default causes the policy to apply to subprojects, this is needed
# to compensate for pecularities in dawn/tint
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
set(CMAKE_POLICY_DEFAULT_CMP0079 NEW)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if (NOT CMAKE_BUILD_TYPE)
    message(WARNING "CMAKE_BUILD_TYPE not set, setting it to RelWithDebInfo so that Dawn doesn't force debug")
    set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Build Type" FORCE)
endif()

# glfw is required to get a bunch of the helper functionality
# if the platform bindings can be made available without glfw
# this can be removed.
#list(APPEND CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}/_deps/glfw-build/src")
include(glfw.cmake)

#------------------------------------------------------------------------------
# Build LabSlang, this will also build Tint and spirv-tools

option(LABSLANG_BUILD_DAWN "" ON)
set(LABSLANG_DAWN_INSTALL_ROOT "C:/tmp/install" CACHE PATH "Location of installed Dawn")
if(LABSLANG_BUILD_DAWN)
    set(DAWN_THIRD_PARTY_DIR "${LABSLANG_ROOT}")
    set(DAWN_VULKAN_DEPS_DIR "${LABSLANG_ROOT}/vulkan-deps")
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

