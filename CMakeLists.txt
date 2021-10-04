cmake_minimum_required(VERSION 3.19)
project(LabSlang)
set(LABSLANG_ROOT "${CMAKE_CURRENT_SOURCE_DIR}")

if (POLICY CMP0077)
    cmake_policy(SET CMP0077 NEW)
endif()

add_subdirectory(SPIRV-Headers)
add_subdirectory(SPIRV-Tools)
set(TINT_BUILD_SPV_READER ON)
add_subdirectory(tint)
add_subdirectory(glm)
add_subdirectory(glfw)
set(DAWN_ABSEIL_DIR "${LABSLANG_ROOT}/abseil-cpp")
add_subdirectory(dawn)

