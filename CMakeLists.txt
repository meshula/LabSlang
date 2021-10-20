cmake_minimum_required(VERSION 3.19)
project(LabSlang)
set(LABSLANG_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

if (POLICY CMP0077)
    cmake_policy(SET CMP0077 NEW)
endif()

#------------------------------------------------------------------------------
# Build LabSlang, this will also build Tint and spirv-tools

option(LABSLANG_BUILD_DAWN "" ON)
set(LABSLANG_DAWN_INSTALL_ROOT "C:/tmp/install" CACHE PATH "Location of installed Dawn")
if(LABSLANG_BUILD_DAWN)
    include(dawn.cmake)
endif()

set(IMGUI_BACKEND_WGPU ON)
set(IMGUI_BACKEND_GLFW ON)
include(imgui.cmake)

set(LABSLANG_DAWN_LIBS
    dawncpp
    dawn_common
    dawn_internal_config
    dawn_native
    dawn_proc
    #dawn_sample_utils
    dawn_utils)

add_executable(LabSlangCppHelloTriangle
    ${CMAKE_SOURCE_DIR}/example/CppHelloTriangle.cpp
    ${CMAKE_SOURCE_DIR}/example/utils.cpp
    ${CMAKE_SOURCE_DIR}/example/utils.h
    )
target_link_libraries(LabSlangCppHelloTriangle
    imgui glfw ${LABSLANG_DAWN_LIBS})
target_include_directories(LabSlangCppHelloTriangle 
    PRIVATE "${LABSLANG_DAWN_INSTALL_ROOT}/include"
    PRIVATE "${LABSLANG_ROOT}/example"
    #PRIVATE "${LABSLANG_ROOT}/dawn/examples"
    PRIVATE "${LABSLANG_ROOT}/dawn/src")
set_target_properties(LabSlangCppHelloTriangle PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY bin)
set_property(TARGET LabSlangCppHelloTriangle PROPERTY FOLDER "LabSlang examples")
install(TARGETS LabSlangCppHelloTriangle
    BUNDLE DESTINATION bin
    RUNTIME DESTINATION bin)

add_executable(LabSlangSwapChain
    ${CMAKE_SOURCE_DIR}/example/ManualSwapChainTest.cpp
    ${CMAKE_SOURCE_DIR}/example/utils.cpp
    ${CMAKE_SOURCE_DIR}/example/utils.h
    )
target_link_libraries(LabSlangSwapChain
    imgui glfw ${LABSLANG_DAWN_LIBS})
target_include_directories(LabSlangSwapChain 
    PRIVATE "${LABSLANG_DAWN_INSTALL_ROOT}/include"
    PRIVATE "${LABSLANG_ROOT}/example"
    #PRIVATE "${LABSLANG_ROOT}/dawn/examples"
    PRIVATE "${LABSLANG_ROOT}/dawn/src")
set_target_properties(LabSlangSwapChain PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY bin)
set_property(TARGET LabSlangSwapChain PROPERTY FOLDER "LabSlang examples")
install(TARGETS LabSlangSwapChain
    BUNDLE DESTINATION bin
    RUNTIME DESTINATION bin)

