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

set(webgpu_gen_headers
    dawn_proc_table.h
    webgpu.h
    webgpu_cpp.h
    webgpu_cpp_print.h)

foreach (H ${webgpu_gen_headers})
    install(FILES ${CMAKE_BINARY_DIR}/dawn/gen/src/include/dawn/${H}
            DESTINATION ${CMAKE_INSTALL_PREFIX}/include/dawn)
endforeach()

# install webgpu.h additionally in the conventional place
install(FILES ${CMAKE_BINARY_DIR}/dawn/gen/src/include/dawn/webgpu.h
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include/webgpu)

set(dawn_headers
    dawn_proc.h
    dawn_thread_dispatch_proc.h
    dawn_wsi.h
    EnumClassBitmasks.h)

foreach (H ${dawn_headers})
    install(FILES ${CMAKE_SOURCE_DIR}/dawn/src/include/dawn/${H}
            DESTINATION ${CMAKE_INSTALL_PREFIX}/include/dawn)
endforeach()

set(dawn_libs
    dawncpp
    dawn_common
    dawn_internal_config
    dawn_native
    dawn_proc
    dawn_sample_utils
    dawn_utils
    dawn_wire)

install(TARGETS dawn_internal_config
    EXPORT dawn_internal_config
    LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
    RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/bin"
    PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_PREFIX}/include")
install(TARGETS dawncpp
    EXPORT dawncpp
    LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
    RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/bin"
    PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_PREFIX}/include")
install(TARGETS dawn_proc
    EXPORT dawn_proc
    LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
    RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/bin"
    PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_PREFIX}/include")
install(TARGETS dawn_common
    EXPORT dawn_common
    LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
    RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/bin"
    PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_PREFIX}/include")
install(TARGETS dawn_native
    EXPORT dawn_native
    LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
    RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/bin"
    PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_PREFIX}/include")
install(TARGETS dawn_wire
    EXPORT dawn_wire
    LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
    RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/bin"
    PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_PREFIX}/include")
install(TARGETS dawn_utils
    EXPORT dawn_utils
    LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
    RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/bin"
    PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_PREFIX}/include")
install(TARGETS dawn_sample_utils
    EXPORT dawn_sample_utils
    LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
    RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/bin"
    PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_PREFIX}/include")
    