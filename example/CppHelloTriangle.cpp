// Copyright 2017 The Dawn Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.


#include "common/Assert.h"
#include "common/Log.h"
#include "common/Platform.h"
#include "common/SystemUtils.h"
#include "utils/BackendBinding.h"
#include "utils/ComboRenderPipelineDescriptor.h"
#include "utils/GLFWUtils.h"
#include "utils/ScopedAutoreleasePool.h"
#include "utils/SystemUtils.h"
#include "utils/TerribleCommandBuffer.h"
#include "utils/WGPUHelpers.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_wgpu.h"

#include <dawn/dawn_proc.h>
#include <dawn/dawn_wsi.h>
#include <dawn_native/DawnNative.h>
#include <dawn_wire/WireClient.h>
#include <dawn_wire/WireServer.h>
#include <GLFW/glfw3.h>
#include <webgpu/webgpu.h>

#include <vector>
#include <algorithm>
#include <cstring>

// Global WebGPU required states
wgpu::Device device;
wgpu::SwapChain swapchain;
static int wgpu_swap_chain_width = 1024;
static int wgpu_swap_chain_height = 768;

wgpu::Buffer indexBuffer;
wgpu::Buffer vertexBuffer;

wgpu::Texture texture;
wgpu::Sampler sampler;

wgpu::Queue queue;
wgpu::TextureView depthStencilView;
wgpu::RenderPipeline pipeline;
wgpu::BindGroup bindGroup;

void PrintGLFWError(int code, const char* message);
void PrintDeviceError(WGPUErrorType errorType, const char* message, void*);


enum class CmdBufType {
    None,
    Terrible,
    // TODO(cwallez@chromium.org): double terrible cmdbuf
};

// Default to D3D12, Metal, Vulkan, OpenGL in that order as D3D12 and Metal are the preferred on
// their respective platforms, and Vulkan is preferred to OpenGL
#if defined(DAWN_ENABLE_BACKEND_D3D12)
static wgpu::BackendType backendType = wgpu::BackendType::D3D12;
#elif defined(DAWN_ENABLE_BACKEND_METAL)
static wgpu::BackendType backendType = wgpu::BackendType::Metal;
#elif defined(DAWN_ENABLE_BACKEND_VULKAN)
static wgpu::BackendType backendType = wgpu::BackendType::Vulkan;
#elif defined(DAWN_ENABLE_BACKEND_OPENGLES)
static wgpu::BackendType backendType = wgpu::BackendType::OpenGLES;
#elif defined(DAWN_ENABLE_BACKEND_DESKTOP_GL)
static wgpu::BackendType backendType = wgpu::BackendType::OpenGL;
#else
#    error
#endif

static CmdBufType cmdBufType = CmdBufType::Terrible;
static std::unique_ptr<dawn_native::Instance> instance;
static utils::BackendBinding* binding = nullptr;

static GLFWwindow* window = nullptr;

static dawn_wire::WireServer* wireServer = nullptr;
static dawn_wire::WireClient* wireClient = nullptr;
static utils::TerribleCommandBuffer* c2sBuf = nullptr;
static utils::TerribleCommandBuffer* s2cBuf = nullptr;

wgpu::Device CreateCppDawnDevice(int width, int height) {
    if (GetEnvironmentVar("ANGLE_DEFAULT_PLATFORM").empty()) {
        SetEnvironmentVar("ANGLE_DEFAULT_PLATFORM", "swiftshader");
    }

    glfwSetErrorCallback(PrintGLFWError);
    if (!glfwInit()) {
        return wgpu::Device();
    }

    // Create the test window and discover adapters using it (esp. for OpenGL)
    utils::SetupGLFWWindowHintsForBackend(backendType);
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);
    window = glfwCreateWindow(width, height, "Dawn window", nullptr, nullptr);
    if (!window) {
        return wgpu::Device();
    }

    instance = std::make_unique<dawn_native::Instance>();
    utils::DiscoverAdapter(instance.get(), window, backendType);

    // Get an adapter for the backend to use, and create the device.
    dawn_native::Adapter backendAdapter;
    {
        std::vector<dawn_native::Adapter> adapters = instance->GetAdapters();
        auto adapterIt = std::find_if(adapters.begin(), adapters.end(),
            [](const dawn_native::Adapter adapter) -> bool {
                wgpu::AdapterProperties properties;
                adapter.GetProperties(&properties);
                return properties.backendType == backendType;
            });
        ASSERT(adapterIt != adapters.end());
        backendAdapter = *adapterIt;
    }

    WGPUDevice backendDevice = backendAdapter.CreateDevice();
    DawnProcTable backendProcs = dawn_native::GetProcs();

    binding = utils::CreateBinding(backendType, window, backendDevice);
    if (binding == nullptr) {
        return wgpu::Device();
    }

    // Choose whether to use the backend procs and devices directly, or set up the wire.
    WGPUDevice cDevice = nullptr;
    DawnProcTable procs;

    switch (cmdBufType) {
    case CmdBufType::None:
        procs = backendProcs;
        cDevice = backendDevice;
        break;

    case CmdBufType::Terrible: {
        c2sBuf = new utils::TerribleCommandBuffer();
        s2cBuf = new utils::TerribleCommandBuffer();

        dawn_wire::WireServerDescriptor serverDesc = {};
        serverDesc.procs = &backendProcs;
        serverDesc.serializer = s2cBuf;

        wireServer = new dawn_wire::WireServer(serverDesc);
        c2sBuf->SetHandler(wireServer);

        dawn_wire::WireClientDescriptor clientDesc = {};
        clientDesc.serializer = c2sBuf;

        wireClient = new dawn_wire::WireClient(clientDesc);
        procs = dawn_wire::client::GetProcs();
        s2cBuf->SetHandler(wireClient);

        auto deviceReservation = wireClient->ReserveDevice();
        wireServer->InjectDevice(backendDevice, deviceReservation.id,
            deviceReservation.generation);

        cDevice = deviceReservation.device;
    } break;
    }

    dawnProcSetProcs(&procs);
    procs.deviceSetUncapturedErrorCallback(cDevice, PrintDeviceError, nullptr);
    return wgpu::Device::Acquire(cDevice);
}

void DoFlush() {
    if (cmdBufType == CmdBufType::Terrible) {
        bool c2sSuccess = c2sBuf->Flush();
        bool s2cSuccess = s2cBuf->Flush();

        ASSERT(c2sSuccess && s2cSuccess);
    }
}


wgpu::TextureFormat GetPreferredSwapChainTextureFormat() {
    DoFlush();
    return static_cast<wgpu::TextureFormat>(binding->GetPreferredSwapChainTextureFormat());
}

wgpu::SwapChain GetSwapChain(const wgpu::Device& device) {
    wgpu::SwapChainDescriptor swapChainDesc = {};
    swapChainDesc.implementation = binding->GetSwapChainImplementation();
    return device.CreateSwapChain(nullptr, &swapChainDesc);
}

wgpu::TextureView CreateDefaultDepthStencilView(const wgpu::Device& device, int width, int height) {
    wgpu::TextureDescriptor descriptor = {};
    descriptor.dimension = wgpu::TextureDimension::e2D;
    descriptor.size.width = width;
    descriptor.size.height = height;
    descriptor.size.depthOrArrayLayers = 1;
    descriptor.sampleCount = 1;
    descriptor.format = wgpu::TextureFormat::Depth24PlusStencil8;
    descriptor.mipLevelCount = 1;
    descriptor.usage = wgpu::TextureUsage::RenderAttachment;
    auto depthStencilTexture = device.CreateTexture(&descriptor);
    return depthStencilTexture.CreateView();
}

bool InitSample(int argc, const char** argv) {
    for (int i = 1; i < argc; i++) {
        if (std::string("-b") == argv[i] || std::string("--backend") == argv[i]) {
            i++;
            if (i < argc && std::string("d3d12") == argv[i]) {
                backendType = wgpu::BackendType::D3D12;
                continue;
            }
            if (i < argc && std::string("metal") == argv[i]) {
                backendType = wgpu::BackendType::Metal;
                continue;
            }
            if (i < argc && std::string("null") == argv[i]) {
                backendType = wgpu::BackendType::Null;
                continue;
            }
            if (i < argc && std::string("opengl") == argv[i]) {
                backendType = wgpu::BackendType::OpenGL;
                continue;
            }
            if (i < argc && std::string("opengles") == argv[i]) {
                backendType = wgpu::BackendType::OpenGLES;
                continue;
            }
            if (i < argc && std::string("vulkan") == argv[i]) {
                backendType = wgpu::BackendType::Vulkan;
                continue;
            }
            fprintf(stderr,
                "--backend expects a backend name (opengl, opengles, metal, d3d12, null, "
                "vulkan)\n");
            return false;
        }
        if (std::string("-c") == argv[i] || std::string("--command-buffer") == argv[i]) {
            i++;
            if (i < argc && std::string("none") == argv[i]) {
                cmdBufType = CmdBufType::None;
                continue;
            }
            if (i < argc && std::string("terrible") == argv[i]) {
                cmdBufType = CmdBufType::Terrible;
                continue;
            }
            fprintf(stderr, "--command-buffer expects a command buffer name (none, terrible)\n");
            return false;
        }
        if (std::string("-h") == argv[i] || std::string("--help") == argv[i]) {
            printf("Usage: %s [-b BACKEND] [-c COMMAND_BUFFER]\n", argv[0]);
            printf("  BACKEND is one of: d3d12, metal, null, opengl, opengles, vulkan\n");
            printf("  COMMAND_BUFFER is one of: none, terrible\n");
            return false;
        }
    }
    return true;
}


bool ShouldQuit() {
    return glfwWindowShouldClose(window);
}

GLFWwindow* GetGLFWWindow() {
    return window;
}





void initBuffers() {
    static const uint32_t indexData[3] = {
        0,
        1,
        2,
    };
    indexBuffer =
        utils::CreateBufferFromData(device, indexData, sizeof(indexData), wgpu::BufferUsage::Index);

    static const float vertexData[12] = {
        0.0f, 0.5f, 0.0f, 1.0f, -0.5f, -0.5f, 0.0f, 1.0f, 0.5f, -0.5f, 0.0f, 1.0f,
    };
    vertexBuffer = utils::CreateBufferFromData(device, vertexData, sizeof(vertexData),
                                               wgpu::BufferUsage::Vertex);
}

void initTextures() {
    wgpu::TextureDescriptor descriptor;
    descriptor.dimension = wgpu::TextureDimension::e2D;
    descriptor.size.width = 1024;
    descriptor.size.height = 1024;
    descriptor.size.depthOrArrayLayers = 1;
    descriptor.sampleCount = 1;
    descriptor.format = wgpu::TextureFormat::RGBA8Unorm;
    descriptor.mipLevelCount = 1;
    descriptor.usage = wgpu::TextureUsage::CopyDst | wgpu::TextureUsage::TextureBinding;
    texture = device.CreateTexture(&descriptor);

    sampler = device.CreateSampler();

    // Initialize the texture with arbitrary data until we can load images
    std::vector<uint8_t> data(4 * 1024 * 1024, 0);
    for (size_t i = 0; i < data.size(); ++i) {
        data[i] = static_cast<uint8_t>(i % 253);
    }

    wgpu::Buffer stagingBuffer = utils::CreateBufferFromData(
        device, data.data(), static_cast<uint32_t>(data.size()), wgpu::BufferUsage::CopySrc);
    wgpu::ImageCopyBuffer imageCopyBuffer =
        utils::CreateImageCopyBuffer(stagingBuffer, 0, 4 * 1024);
    wgpu::ImageCopyTexture imageCopyTexture = utils::CreateImageCopyTexture(texture, 0, {0, 0, 0});
    wgpu::Extent3D copySize = {1024, 1024, 1};

    wgpu::CommandEncoder encoder = device.CreateCommandEncoder();
    encoder.CopyBufferToTexture(&imageCopyBuffer, &imageCopyTexture, &copySize);

    wgpu::CommandBuffer copy = encoder.Finish();
    queue.Submit(1, &copy);
}

void init() {
    device = CreateCppDawnDevice(wgpu_swap_chain_width, wgpu_swap_chain_height);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = NULL;
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOther(GetGLFWWindow(), true);
    ImGui_ImplWGPU_Init(device.Get(), 3, WGPUTextureFormat_RGBA8Unorm);

    queue = device.GetQueue();
    swapchain = GetSwapChain(device);
    swapchain.Configure(GetPreferredSwapChainTextureFormat(), wgpu::TextureUsage::RenderAttachment,
        wgpu_swap_chain_width,
        wgpu_swap_chain_height);

    initBuffers();
    initTextures();

    wgpu::ShaderModule vsModule = utils::CreateShaderModule(device, R"(
        [[stage(vertex)]] fn main([[location(0)]] pos : vec4<f32>)
                               -> [[builtin(position)]] vec4<f32> {
            return pos;
        })");

    wgpu::ShaderModule fsModule = utils::CreateShaderModule(device, R"(
        [[group(0), binding(0)]] var mySampler: sampler;
        [[group(0), binding(1)]] var myTexture : texture_2d<f32>;

        [[stage(fragment)]] fn main([[builtin(position)]] FragCoord : vec4<f32>)
                                 -> [[location(0)]] vec4<f32> {
            return textureSample(myTexture, mySampler, FragCoord.xy / vec2<f32>(640.0, 480.0));
        })");

    auto bgl = utils::MakeBindGroupLayout(
        device, {
                    {0, wgpu::ShaderStage::Fragment, wgpu::SamplerBindingType::Filtering},
                    {1, wgpu::ShaderStage::Fragment, wgpu::TextureSampleType::Float},
                });

    wgpu::PipelineLayout pl = utils::MakeBasicPipelineLayout(device, &bgl);

    depthStencilView = CreateDefaultDepthStencilView(device, wgpu_swap_chain_width, wgpu_swap_chain_height);

    utils::ComboRenderPipelineDescriptor descriptor;
    descriptor.layout = utils::MakeBasicPipelineLayout(device, &bgl);
    descriptor.vertex.module = vsModule;
    descriptor.vertex.bufferCount = 1;
    descriptor.cBuffers[0].arrayStride = 4 * sizeof(float);
    descriptor.cBuffers[0].attributeCount = 1;
    descriptor.cAttributes[0].format = wgpu::VertexFormat::Float32x4;
    descriptor.cFragment.module = fsModule;
    descriptor.cTargets[0].format = GetPreferredSwapChainTextureFormat();
    descriptor.EnableDepthStencil(wgpu::TextureFormat::Depth24PlusStencil8);

    pipeline = device.CreateRenderPipeline(&descriptor);

    wgpu::TextureView view = texture.CreateView();

    bindGroup = utils::MakeBindGroup(device, bgl, {{0, sampler}, {1, view}});
}


void frame() 
{
    glfwPollEvents();

    int width, height;
    glfwGetFramebufferSize(GetGLFWWindow(), &width, &height);

    // React to changes in screen size
    if (width != wgpu_swap_chain_width && height != wgpu_swap_chain_height)
    {
        ImGui_ImplWGPU_InvalidateDeviceObjects();
        if (swapchain.Get())
            wgpuSwapChainRelease(swapchain.Get());

        wgpu_swap_chain_width = width;
        wgpu_swap_chain_height = height;
        depthStencilView.Release();
        depthStencilView = CreateDefaultDepthStencilView(device, width, height);

        wgpu::SwapChainDescriptor swapChainDesc = {};
        swapchain = device.CreateSwapChain(nullptr, &swapChainDesc);
        swapchain.Configure(
            wgpu::TextureFormat::Depth24PlusStencil8, 
            wgpu::TextureUsage::RenderAttachment,
            wgpu_swap_chain_width,
            wgpu_swap_chain_height);

        ImGui_ImplWGPU_CreateDeviceObjects();
        DoFlush();
    }

    wgpu::TextureView backbufferView = swapchain.GetCurrentTextureView();
    utils::ComboRenderPassDescriptor renderPass({backbufferView}, depthStencilView);

    wgpu::CommandEncoder encoder = device.CreateCommandEncoder();
    {
        wgpu::RenderPassEncoder pass = encoder.BeginRenderPass(&renderPass);
        pass.SetPipeline(pipeline);
        pass.SetBindGroup(0, bindGroup);
        pass.SetVertexBuffer(0, vertexBuffer);
        pass.SetIndexBuffer(indexBuffer, wgpu::IndexFormat::Uint32);
        pass.DrawIndexed(3);
        pass.EndPass();
    }

    wgpu::CommandBuffer commands = encoder.Finish();
    queue.Submit(1, &commands);



//////////////

    ImGui_ImplWGPU_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    static bool show_demo_window = true;
    ImGui::ShowDemoWindow(&show_demo_window);

    ImGui::Render();

    //........................

    static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    WGPURenderPassColorAttachment color_attachments = {};
    color_attachments.loadOp = WGPULoadOp_Clear;
    color_attachments.storeOp = WGPUStoreOp_Store;
    color_attachments.clearColor = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
    color_attachments.view = wgpuSwapChainGetCurrentTextureView(swapchain.Get());
    WGPURenderPassDescriptor render_pass_desc = {};
    render_pass_desc.colorAttachmentCount = 1;
    render_pass_desc.colorAttachments = &color_attachments;
    render_pass_desc.depthStencilAttachment = NULL;

    WGPUCommandEncoderDescriptor enc_desc = {};
    WGPUCommandEncoder im_encoder = wgpuDeviceCreateCommandEncoder(device.Get(), &enc_desc);

    WGPURenderPassEncoder pass = wgpuCommandEncoderBeginRenderPass(im_encoder, &render_pass_desc);
    ImGui_ImplWGPU_RenderDrawData(ImGui::GetDrawData(), pass);
    wgpuRenderPassEncoderEndPass(pass);

    WGPUCommandBufferDescriptor cmd_buffer_desc = {};
    WGPUCommandBuffer cmd_buffer = wgpuCommandEncoderFinish(im_encoder, &cmd_buffer_desc);
    WGPUQueue queue = wgpuDeviceGetQueue(device.Get());
    wgpuQueueSubmit(queue, 1, &cmd_buffer);

//////////////////



    swapchain.Present();
    DoFlush();
}

int main(int argc, const char* argv[]) {
    if (!InitSample(argc, argv)) {
        return 1;
    }
    init();

    while (!ShouldQuit()) {
        utils::ScopedAutoreleasePool pool;
        frame();
        utils::USleep(16000);
    }

    // TODO release stuff
}


void PrintGLFWError(int code, const char* message) {
    dawn::ErrorLog() << "GLFW error: " << code << " - " << message;
}

void PrintDeviceError(WGPUErrorType errorType, const char* message, void*) {
    const char* errorTypeName = "";
    switch (errorType) {
    case WGPUErrorType_Validation:
        errorTypeName = "Validation";
        break;
    case WGPUErrorType_OutOfMemory:
        errorTypeName = "Out of memory";
        break;
    case WGPUErrorType_Unknown:
        errorTypeName = "Unknown";
        break;
    case WGPUErrorType_DeviceLost:
        errorTypeName = "Device lost";
        break;
    default:
        UNREACHABLE();
        return;
    }
    dawn::ErrorLog() << errorTypeName << " error: " << message;
}
