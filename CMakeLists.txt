cmake_minimum_required(VERSION 3.19)
project(LabSlang)
set(LABSLANG_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
option(LABSLANG_BUILD_DAWN "" ON)
set(LABSLANG_DAWN_INSTALL_ROOT "C:/tmp/install" CACHE PATH "Location of installed Dawn")
if(LABSLANG_BUILD_DAWN)
    include(dawn.cmake)
endif()

set(IMGUI_BACKEND_WGPU ON)
include(imgui.cmake)

set(LABSLANG_DAWN_LIBS
    dawncpp
    dawn_common
    dawn_internal_config
    dawn_native
    dawn_proc
    dawn_sample_utils
    dawn_utils)
add_executable(LabSlangCppHelloTriangle
    ${CMAKE_SOURCE_DIR}/dawn/examples/CppHelloTriangle.cpp)
target_link_libraries(LabSlangCppHelloTriangle
    glfw ${LABSLANG_DAWN_LIBS})
target_include_directories(LabSlangCppHelloTriangle 
    PRIVATE "${LABSLANG_DAWN_INSTALL_ROOT}/include"
    PRIVATE "${LABSLANG_ROOT}/dawn/src")
set_target_properties(LabSlangCppHelloTriangle PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY bin)
set_property(TARGET LabSlangCppHelloTriangle PROPERTY FOLDER "LabSlang examples")
install(TARGETS LabSlangCppHelloTriangle
    BUNDLE DESTINATION bin
    RUNTIME DESTINATION bin)

