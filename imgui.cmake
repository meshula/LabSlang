
# adapteed from https://github.com/ocornut/imgui/pull/4614 by yhsb2k

option(IMGUI_BACKEND_ALLEGRO5 "Allegro Gaming Library")
option(IMGUI_BACKEND_ANDROID "Android native API")
option(IMGUI_BACKEND_DX9 "DirectX9")
option(IMGUI_BACKEND_DX10 "DirectX10")
option(IMGUI_BACKEND_DX11 "DirectX11")
option(IMGUI_BACKEND_DX12 "DirectX12")
option(IMGUI_BACKEND_GLFW "GLFW (Windows, macOS, Linux, etc.)")
option(IMGUI_BACKEND_GLUT "GLUT/FreeGLUT (legacy, not recommended today)")
option(IMGUI_BACKEND_MARMALADE "Marmalade SDK")
option(IMGUI_BACKEND_METAL "Metal (with ObjC)")
option(IMGUI_BACKEND_OPENGL2 "OpenGL 2 (legacy, not recommended today)")
option(IMGUI_BACKEND_OPENGL3 "OpenGL 3/4")
option(IMGUI_BACKEND_OSX "macOS native API")
option(IMGUI_BACKEND_SDL "SDL2 (Windows, macOS, Linux, iOS, Android)")
option(IMGUI_BACKEND_SDL_RENDERER "SDL2 renderer")
option(IMGUI_BACKEND_VULKAN "Vulkan")
option(IMGUI_BACKEND_WGPU "WebGPU")
option(IMGUI_BACKEND_DAWN "Dawn")
option(IMGUI_BACKEND_WINAPI "Win32 native API")

add_library(imgui
    imgui/imgui.cpp
    imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    $<$<BOOL:${IMGUI_BACKEND_ALLEGRO5}>:imgui/backends/imgui_impl_allegro5.cpp>
    $<$<BOOL:${IMGUI_BACKEND_ANDROID}>:imgui/backends/imgui_impl_android.cpp>
    $<$<BOOL:${IMGUI_BACKEND_DX9}>:imgui/backends/imgui_impl_dx9.cpp>
    $<$<BOOL:${IMGUI_BACKEND_DX10}>:imgui/backends/imgui_impl_dx10.cpp>
    $<$<BOOL:${IMGUI_BACKEND_DX11}>:imgui/backends/imgui_impl_dx11.cpp>
    $<$<BOOL:${IMGUI_BACKEND_DX12}>:imgui/backends/imgui_impl_dx12.cpp>
    $<$<BOOL:${IMGUI_BACKEND_GLFW}>:imgui/backends/imgui_impl_glfw.cpp>
    $<$<BOOL:${IMGUI_BACKEND_GLUT}>:imgui/backends/imgui_impl_glut.cpp>
    $<$<BOOL:${IMGUI_BACKEND_MARMALADE}>:imgui/backends/imgui_impl_marmalade.cpp>
    $<$<BOOL:${IMGUI_BACKEND_METAL}>:imgui/backends/imgui_impl_metal.mm>
    $<$<BOOL:${IMGUI_BACKEND_OPENGL2}>:imgui/backends/imgui_impl_opengl2.cpp>
    $<$<BOOL:${IMGUI_BACKEND_OPENGL3}>:imgui/backends/imgui_impl_opengl3.cpp>
    $<$<BOOL:${IMGUI_BACKEND_OSX}>:imgui/backends/imgui_impl_osx.mm>
    $<$<BOOL:${IMGUI_BACKEND_SDL}>:imgui/backends/imgui_impl_sdl.cpp>
    $<$<BOOL:${IMGUI_BACKEND_SDL_RENDERER}>:imgui/backends/imgui_impl_sdlrenderer.cpp>
    $<$<BOOL:${IMGUI_BACKEND_VULKAN}>:imgui/backends/imgui_impl_vulkan.cpp>
    $<$<BOOL:${IMGUI_BACKEND_WGPU}>:imgui/backends/imgui_impl_wgpu.cpp>
    $<$<BOOL:${IMGUI_BACKEND_WINAPI}>:imgui/backends/imgui_impl_win32.cpp>
)

set_property(TARGET imgui PROPERTY CXX_STANDARD 11)

target_include_directories(imgui
    PUBLIC
        imgui
        imgui/backends
    PRIVATE 
        "${LABSLANG_DAWN_INSTALL_ROOT}/include"
        "${WEBGPU_HEADER_LOCATION}"
)

if (IMGUI_BACKEND_DAWN)
    add_dependencies(imgui webgpu_header)
endif()

target_link_libraries(imgui
    PRIVATE
        $<$<BOOL:${IMGUI_BACKEND_ANDROID}>:android>
        $<$<BOOL:${IMGUI_BACKED_DAWN}>:dawn_native>
        $<$<BOOL:${IMGUI_BACKEND_DX9}>:d3d9.lib>
        $<$<BOOL:${IMGUI_BACKEND_DX10}>:d3d10.lib>
        $<$<BOOL:${IMGUI_BACKEND_DX11}>:d3d11.lib>
        $<$<BOOL:${IMGUI_BACKEND_DX12}>:d3d12.lib>
        $<$<BOOL:${IMGUI_BACKEND_DX10}>:d3dcompiler.lib>
        $<$<BOOL:${IMGUI_BACKEND_DX11}>:d3dcompiler.lib>
        $<$<BOOL:${IMGUI_BACKEND_DX12}>:d3dcompiler.lib>
        $<$<BOOL:${IMGUI_BACKEND_DX12}>:dxgi.lib>
        $<$<BOOL:${IMGUI_BACKEND_GLFW}>:glfw>
        $<$<BOOL:${IMGUI_BACKEND_GLUT}>:glut>
        $<$<BOOL:${IMGUI_BACKEND_METAL}>:"-framework Metal -framework MetalKit -framework QuartzCore">
        $<$<BOOL:${IMGUI_BACKEND_OSX}>:"-framework Cocoa">
        $<$<OR:$<BOOL:${IMGUI_BACKEND_SDL}>,$<BOOL:${IMGUI_BACKEND_SDL_RENDERER}>>:SDL2::SDL2main>
        $<$<OR:$<BOOL:${IMGUI_BACKEND_SDL}>,$<BOOL:${IMGUI_BACKEND_SDL_RENDERER}>>:$<IF:$<BOOL:${SDL_STATIC_ENABLED_BY_DEFAULT}>,SDL2::SDL2-static,SDL2::SDL2>>
        $<$<BOOL:${IMGUI_BACKEND_VULKAN}>:Vulkan::Vulkan>
        $<$<AND:$<BOOL:${IMGUI_BACKEND_OPENGL3}>,$<BOOL:${OpenGL_FOUND}>>:OpenGL::OpenGL>
        $<$<AND:$<BOOL:${IMGUI_BACKEND_OPENGL2}>,$<BOOL:${OpenGL_FOUND}>>:OpenGL::OpenGL>
)

