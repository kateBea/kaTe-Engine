//
// Created by kate on 6/29/23.
//

#include <memory>
#include <vector>
#include <unordered_set>
#include <set>

#include "glm/glm.hpp"

#include "Tools/Common.hh"

#include "Core/Assert.hh"
#include "Core/Logger.hh"
#include "Core/Application.hh"
#include "Renderer/RendererAPI.hh"

#include <Platform/Window/MainWindow.hh>
#include "Renderer/Buffers/IndexBuffer.hh"
#include "Renderer/Buffers/VertexBuffer.hh"
#include "Renderer/Material/BaseShader.hh"

#include "Renderer/Vulkan/VulkanVertexBuffer.hh"

#include "Renderer/Vulkan/VulkanRenderer.hh"

namespace kaTe {
    static auto DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks *pAllocator) -> void {
        auto func{ (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT") };

        if (func != nullptr)
            func(instance, debugMessenger, pAllocator);
    }

    auto VulkanRenderer::Init() -> void {
        // For now it expects a MainWindow which is a wrapper around a GLFW window
        m_Window = std::dynamic_pointer_cast<MainWindow>(Application::GetPtr()->GetMainWindowPtr());

        CreateInstance();
        SetupDebugMessenger();
        CreateSurface();
        PickPhysicalDevice();
        CreateLogicalDevice();
        CreateCommandPool();

        m_WindowExtent = m_Window->GetExtent();

        CreateSwapChain();
        CreateImageViews();
        CreateRenderPass();
        CreateDepthResources();
        CreateFrameBuffers();
        CreateSyncObjects();

        CreatePipelineLayout();
        RecreateSwapChain();
        CreateCommandBuffers();
    }

    auto VulkanRenderer::Shutdown() -> void {
        // Wait until all operations have completed
        vkDeviceWaitIdle(m_Device);

        DestroySwapChain();

        vkDestroyShaderModule(m_Device, m_VertShaderModule, nullptr);
        vkDestroyShaderModule(m_Device, m_FragShaderModule, nullptr);
        vkDestroyPipeline(m_Device, m_GraphicsPipeline, nullptr);

        vkDestroyCommandPool(m_Device, m_CommandPool, nullptr);
        vkDestroyDevice(m_Device, nullptr);

        if (m_EnableValidationLayers)
            DestroyDebugUtilsMessengerEXT(m_Instance, m_DebugMessenger, nullptr);

        vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);
        vkDestroyInstance(m_Instance, nullptr);
    }

    auto VulkanRenderer::EnableWireframeMode() -> void {

    }

    auto VulkanRenderer::DisableWireframeMode() -> void {

    }

    auto VulkanRenderer::SetClearColor(const glm::vec4& color) -> void {
        m_ClearColor = color;
    }

    auto VulkanRenderer::SetClearColor(float red, float green, float blue, float alpha) -> void {
        m_ClearColor = glm::vec4(red, green, blue, alpha);
    }

    auto VulkanRenderer::Clear(const RendererAPI::BufferBits &bufferBits) -> void {

    }

    auto VulkanRenderer::SetViewPort(UInt32_T x, UInt32_T y, UInt32_T width, UInt32_T height) -> void {

    }

    auto VulkanRenderer::DrawIndexed(const std::shared_ptr<VertexBuffer> &vertexBuffer, const std::shared_ptr<IndexBuffer> &indexBuffer) -> void {

    }

    auto VulkanRenderer::DrawIndexed(const std::shared_ptr<BaseShader> &shader, const std::shared_ptr<VertexBuffer> &vertexBuffer, const std::shared_ptr<IndexBuffer> &indexBuffer) -> void {

    }

    auto VulkanRenderer::Draw(const std::shared_ptr<VertexBuffer>& vertexBuffer) -> void {
        UInt32_T imageIndex{};

        auto result{ AcquireNextImage(&imageIndex) };
        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            RecreateSwapChain();
            return;
        }
        else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("failed to acquire swap chain image!");
        }

        RecordCommandBuffers(imageIndex, vertexBuffer);
        result = SubmitCommandBuffers(&m_CommandBuffers[imageIndex], &imageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
            RecreateSwapChain();
            return;
        }
        else if (result != VK_SUCCESS) {
            throw std::runtime_error("failed to present swap chain image!");
        }
    }

    auto VulkanRenderer::Draw(const std::shared_ptr<VertexBuffer> &vertexBuffer, const std::shared_ptr<IndexBuffer> &indexBuffer) -> void {

    }

    auto VulkanRenderer::Draw(const std::shared_ptr<BaseShader> &shader, const std::shared_ptr<VertexBuffer> &vertexBuffer) -> void {

    }

    auto VulkanRenderer::Draw(const std::shared_ptr<BaseShader> &shader, const std::shared_ptr<VertexBuffer> &vertexBuffer, const std::shared_ptr<IndexBuffer> &indexBuffer) -> void {

    }

    auto VulkanRenderer::SetDefaultShader(const Path_T& vertShaderPath, const Path_T& pixelShaderPath) -> void {

    }

    auto VulkanRenderer::CreatePipelineLayout() -> void {
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};

        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.pPushConstantRanges = nullptr;

        if (vkCreatePipelineLayout(m_Device, &pipelineLayoutInfo, nullptr, &m_PipelineLayout) != VK_SUCCESS)
            throw std::runtime_error("Failed to create pipeline layout");
    }

    auto VulkanRenderer::CreatePipeline() -> void {
        auto pipelineConfig{ GetDefaultPipelineConfigInfo() };

        pipelineConfig.renderPass = m_RenderPass;
        pipelineConfig.pipelineLayout = m_PipelineLayout;

        std::string vPath{ "../assets/shaders/basicVert.sprv" };
        std::string fPath{ "../assets/shaders/basicFrag.sprv" };
        CreateGraphicsPipeline(vPath, fPath, pipelineConfig);
    }

    auto VulkanRenderer::CreateCommandBuffers() -> void {
        m_CommandBuffers.resize(GetImageCount());

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = GetCommandPool();
        allocInfo.commandBufferCount = static_cast<UInt32_T>(m_CommandBuffers.size());

        if (vkAllocateCommandBuffers(m_Device, &allocInfo, m_CommandBuffers.data()) != VK_SUCCESS)
            throw std::runtime_error("Failed to allocate command buffers");
    }

    auto VulkanRenderer::InitSwapChain() -> void {
        CreateSwapChain();
        CreateImageViews();
        CreateRenderPass();
        CreateDepthResources();
        CreateFrameBuffers();
        CreateSyncObjects();
    }

    auto VulkanRenderer::DestroySwapChain() -> void {
        for (auto imageView: m_SwapChainImageViews)
            vkDestroyImageView(m_Device, imageView, nullptr);

        m_SwapChainImageViews.clear();
        vkDestroySwapchainKHR(m_Device, m_SwapChain, nullptr);

        for (std::size_t i{}; i < m_DepthImages.size(); i++) {
            vkDestroyImageView(m_Device, m_DepthImageViews[i], nullptr);
            vkDestroyImage(m_Device, m_DepthImages[i], nullptr);
            vkFreeMemory(m_Device, m_DepthImageMemories[i], nullptr);
        }

        for (auto framebuffer: m_SwapChainFrameBuffers)
            vkDestroyFramebuffer(m_Device, framebuffer, nullptr);

        vkDestroyRenderPass(m_Device, m_RenderPass, nullptr);

        // cleanup synchronization objects
        for (std::size_t i{}; i < MAX_FRAMES_IN_FLIGHT; i++) {
            vkDestroySemaphore(m_Device, m_RenderFinishedSemaphores[i], nullptr);
            vkDestroySemaphore(m_Device, m_ImageAvailableSemaphores[i], nullptr);
            vkDestroyFence(m_Device, m_InFlightFences[i], nullptr);
        }
    }

    auto VulkanRenderer::RecreateSwapChain() -> void {
        vkDeviceWaitIdle(m_Device);
        // Wait for the current swap chain to stop being used
        auto extent{ m_Window->GetExtent() };

        // do not process event's while this window has no size, e.g. when minimized
        while (extent.width == 0 || extent.height == 0) {
            extent = m_Window->GetExtent();
            glfwWaitEvents();
        }


        DestroySwapChain();
        InitSwapChain();

        static bool first{ true };
        // No need if render passes from swap chain and pipeline are compatible
        if (first) {
            first = false;
            CreatePipeline();
        }
    }

    auto VulkanRenderer::RecordCommandBuffers(UInt32_T imageIndex, const std::shared_ptr<VertexBuffer>& vertexBuffer) -> void {
        VkCommandBufferBeginInfo beginInfo{};

        auto vertices{ std::dynamic_pointer_cast<VulkanVertexBuffer>(vertexBuffer) };

        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(m_CommandBuffers[imageIndex], &beginInfo) != VK_SUCCESS)
            throw std::runtime_error("Failed to begin recording command buffer");

        VkRenderPassBeginInfo renderPassInfo{};

        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = GetRenderPass();
        renderPassInfo.framebuffer = GetFrameBuffer(imageIndex);

        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.renderArea.extent = GetSwapChainExtent();

        std::array<VkClearValue, 2> clearValues{};

        clearValues[0].color = { {m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a} };
        clearValues[1].depthStencil = { 1.0f, 0 };

        renderPassInfo.clearValueCount = static_cast<UInt32_T>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(m_CommandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(GetSwapChainExtent().width);
        viewport.height = static_cast<float>(GetSwapChainExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        VkRect2D scissor{ .offset{ 0, 0 }, .extent{ GetSwapChainExtent() } };
        vkCmdSetViewport(m_CommandBuffers[imageIndex], 0, 1, &viewport);
        vkCmdSetScissor(m_CommandBuffers[imageIndex], 0, 1, &scissor);

        Bind(m_CommandBuffers[imageIndex]);
        vertices->Bind(m_CommandBuffers[imageIndex]);

        // draw call
        vkCmdDraw(m_CommandBuffers[imageIndex], vertices->GetVertexCount() , 1, 0, 0);

        vkCmdEndRenderPass((m_CommandBuffers[imageIndex]));

        if (vkEndCommandBuffer(m_CommandBuffers[imageIndex]) != VK_SUCCESS)
            throw std::runtime_error("Failed to record command buffer");
    }

    auto CreateDebugUtilsMessengerEXT(
            VkInstance instance,
            const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
            const VkAllocationCallbacks *pAllocator,
            VkDebugUtilsMessengerEXT *pDebugMessenger) -> VkResult
    {
        auto func { (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT") };

        if (func != nullptr)
            return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
        else
            return VK_ERROR_EXTENSION_NOT_PRESENT;

    }

    auto VulkanRenderer::CreateInstance() -> void {
        if (m_EnableValidationLayers && !CheckValidationLayerSupport())
            throw std::runtime_error("Validation layers requested, but not available!");

        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Vulkate";
        appInfo.applicationVersion = VK_MAKE_API_VERSION(1, 0, 0, 0);
        appInfo.pEngineName = "kaTe Engine";
        appInfo.engineVersion = VK_MAKE_API_VERSION(1, 0, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        auto extensions{ GetRequiredExtensions() };
        createInfo.enabledExtensionCount = static_cast<UInt32_T>(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data();

        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
        if (m_EnableValidationLayers) {
            createInfo.enabledLayerCount = static_cast<UInt32_T>(m_ValidationLayers.size());
            createInfo.ppEnabledLayerNames = m_ValidationLayers.data();

            PopulateDebugMessengerCreateInfo(debugCreateInfo);
            createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
        }

        if (vkCreateInstance(&createInfo, nullptr, &m_Instance) != VK_SUCCESS)
            throw std::runtime_error("failed to create instance!");

        volkLoadInstance(m_Instance);

        HasGflwRequiredInstanceExtensions();
    }

    auto VulkanRenderer::PickPhysicalDevice() -> void{
        UInt32_T deviceCount {};
        vkEnumeratePhysicalDevices(m_Instance, &deviceCount, nullptr);

        if (deviceCount == 0)
            throw std::runtime_error("failed to find GPUs with Vulkan support!");

        KT_COLOR_PRINT_FORMATTED(KT_FMT_COLOR_GREEN_YELLOW, "Device count: {}\n", deviceCount);
        std::vector<VkPhysicalDevice> devices(deviceCount);

        vkEnumeratePhysicalDevices(m_Instance, &deviceCount, devices.data());

        m_PhysicalDevice = VK_NULL_HANDLE;
        for (const auto& device : devices) {
            if (IsDeviceSuitable(device)) {
                m_PhysicalDevice = device;
                break;
            }
        }

        if (m_PhysicalDevice == VK_NULL_HANDLE)
            throw std::runtime_error("failed to find a suitable GPU!");

        vkGetPhysicalDeviceProperties(m_PhysicalDevice, &m_Properties);
        KT_COLOR_PRINT_FORMATTED(KT_FMT_COLOR_GREEN_YELLOW, "Physical device: {}\n", m_Properties.deviceName);
    }

    auto VulkanRenderer::CreateLogicalDevice() -> void {
        QueueFamilyIndices indices{ FindQueueFamilies(m_PhysicalDevice) };

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos{};
        std::set<UInt32_T> uniqueQueueFamilies{ indices.GraphicsFamily, indices.PresentFamily };

        constexpr float queuePriority{ 1.0f };
        for (UInt32_T queueFamily : uniqueQueueFamilies) {
            VkDeviceQueueCreateInfo queueCreateInfo{};

            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;

            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkPhysicalDeviceFeatures deviceFeatures{};
        deviceFeatures.samplerAnisotropy = VK_TRUE;

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

        createInfo.queueCreateInfoCount = static_cast<UInt32_T>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();

        createInfo.pEnabledFeatures = &deviceFeatures;
        createInfo.enabledExtensionCount = static_cast<UInt32_T>(m_DeviceRequiredExtensions.size());
        createInfo.ppEnabledExtensionNames = m_DeviceRequiredExtensions.data();

        // might not be necessary anymore because device-specific validation layers have been deprecated
        // even tho recommended for some backwards compatibility as they are required for some Vulkan implementations
        if (m_EnableValidationLayers) {
            // These two fields are ignored by up-to-date Vulkan implementations
            createInfo.enabledLayerCount = static_cast<UInt32_T>(m_ValidationLayers.size());
            createInfo.ppEnabledLayerNames = m_ValidationLayers.data();
        }
        else {
            createInfo.enabledLayerCount = 0;
        }

        if (vkCreateDevice(m_PhysicalDevice, &createInfo, nullptr, &m_Device) != VK_SUCCESS)
            throw std::runtime_error("failed to create logical device!");

        vkGetDeviceQueue(m_Device, indices.GraphicsFamily, 0, &m_GraphicsQueue);
        vkGetDeviceQueue(m_Device, indices.PresentFamily, 0, &m_PresentQueue);
    }

    auto VulkanRenderer::CreateCommandPool() -> void {
        QueueFamilyIndices queueFamilyIndices{ FindPhysicalQueueFamilies() };

        VkCommandPoolCreateInfo poolInfo{};

        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        poolInfo.queueFamilyIndex = queueFamilyIndices.GraphicsFamily;

        if (vkCreateCommandPool(m_Device, &poolInfo, nullptr, &m_CommandPool) != VK_SUCCESS)
            throw std::runtime_error("failed to create command pool!");
    }

    auto VulkanRenderer::CreateSurface() -> void {
        m_Window->CreateWindowSurface(m_Instance, &m_Surface);
    }

    auto VulkanRenderer::IsDeviceSuitable(VkPhysicalDevice device) -> bool {
        QueueFamilyIndices indices{ FindQueueFamilies(device) };
        bool extensionsSupported{ CheckDeviceExtensionSupport(device) };
        bool swapChainAdequate{ false };

        if (extensionsSupported) {
            SwapChainSupportDetails swapChainSupport{ QuerySwapChainSupport(device) };
            swapChainAdequate = !swapChainSupport.Formats.empty() && !swapChainSupport.PresentModes.empty();
        }

        VkPhysicalDeviceFeatures supportedFeatures{};
        vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

        return indices.IsComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
    }

    auto VulkanRenderer::PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) -> void {
        createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;

        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                     VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

        createInfo.pfnUserCallback = [](
                                             VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                             VkDebugUtilsMessageTypeFlagsEXT messageType,
                                             const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) -> VKAPI_ATTR VkBool32 VKAPI_CALL
        {
            KATE_CORE_LOGGER_ERROR("validation layer: {}", pCallbackData->pMessage);
            return VK_FALSE;
        };
    }

    auto VulkanRenderer::SetupDebugMessenger() -> void {
        if (!m_EnableValidationLayers)
            return;

        VkDebugUtilsMessengerCreateInfoEXT createInfo{};
        PopulateDebugMessengerCreateInfo(createInfo);
        if (CreateDebugUtilsMessengerEXT(m_Instance, &createInfo, nullptr, &m_DebugMessenger) != VK_SUCCESS)
            throw std::runtime_error("failed to set up debug messenger!");
    }

    auto VulkanRenderer::CheckValidationLayerSupport() -> bool {
        UInt32_T layerCount{};
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (const char* layerName : m_ValidationLayers) {
            bool layerFound{ false };

            for (const auto& layerProperties : availableLayers) {
                if (std::string(layerName) == std::string(layerProperties.layerName)) {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound) return false;

        }

        return true;
    }

    auto VulkanRenderer::GetRequiredExtensions() const -> std::vector<const char*> {
        UInt32_T glfwExtensionCount{};
        const char** glfwExtensions{ glfwGetRequiredInstanceExtensions(&glfwExtensionCount) };
        std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

        if (m_EnableValidationLayers)
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

        return extensions;
    }

    auto VulkanRenderer::HasGflwRequiredInstanceExtensions() -> void {
        UInt32_T extensionCount{};
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
        std::vector<VkExtensionProperties> extensions(extensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

        KT_COLOR_PRINT_FORMATTED(KT_FMT_COLOR_AQUA, "Available extensions: \n");
        std::unordered_set<std::string> available{};
        for (const auto& extension : extensions) {
            KT_COLOR_PRINT_FORMATTED(KT_FMT_COLOR_GREEN_YELLOW, "\t{}\n", extension.extensionName);
            available.insert(extension.extensionName);
        }

        KT_COLOR_PRINT_FORMATTED(KT_FMT_COLOR_AQUA, "Required extensions: \n");
        auto requiredExtensions{ GetRequiredExtensions() };
        for (const auto& required: requiredExtensions) {
            KT_COLOR_PRINT_FORMATTED(KT_FMT_COLOR_ORANGE_RED, "\t{}\n", required);
            if (available.find(required) == available.end()) {
                throw std::runtime_error("Missing required GLFW extensions");
            }
        }
    }

    auto VulkanRenderer::CheckDeviceExtensionSupport(VkPhysicalDevice device) -> bool {
        UInt32_T extensionCount{};
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

        UInt32_T matchedExtensions{};

        std::string required{};
        std::string supported{};

        for (const auto& reqExtension : m_DeviceRequiredExtensions) {
            required = reqExtension;
            for (const auto& extension : availableExtensions) {
                matchedExtensions += required == std::string(extension.extensionName) ? 1 : 0;
            }
        }

        return matchedExtensions == m_DeviceRequiredExtensions.size();
    }

    auto VulkanRenderer::FindQueueFamilies(VkPhysicalDevice device) -> QueueFamilyIndices {
        QueueFamilyIndices indices{};

        UInt32_T queueFamilyCount{};
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        Int32_T i{};
        for (const auto& queueFamily : queueFamilies) {
            if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                indices.GraphicsFamily = i;
                indices.GraphicsFamilyHasValue = true;
            }

            VkBool32 presentSupport{ VK_FALSE };
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_Surface, &presentSupport);
            if (queueFamily.queueCount > 0 && presentSupport) {
                indices.PresentFamily = i;
                indices.PresentFamilyHasValue = true;
            }

            if (indices.IsComplete())
                break;

            i++;
        }

        return indices;
    }

    auto VulkanRenderer::QuerySwapChainSupport(VkPhysicalDevice device) -> SwapChainSupportDetails {
        SwapChainSupportDetails details{};
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, m_Surface, &details.Capabilities);

        UInt32_T formatCount{};
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_Surface, &formatCount, nullptr);
        if (formatCount != 0) {
            details.Formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_Surface, &formatCount, details.Formats.data());
        }

        UInt32_T presentModeCount{};
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_Surface, &presentModeCount, nullptr);
        if (presentModeCount != 0) {
            details.PresentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_Surface, &presentModeCount, details.PresentModes.data());
        }

        return details;
    }

    auto VulkanRenderer::FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) -> VkFormat {
        for (VkFormat format : candidates) {
            VkFormatProperties props{};
            vkGetPhysicalDeviceFormatProperties(m_PhysicalDevice, format, &props);

            if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features)
                return format;
            else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
                return format;

        }
        throw std::runtime_error("failed to find supported format!");
    }

    auto VulkanRenderer::FindMemoryType(UInt32_T typeFilter, VkMemoryPropertyFlags properties) -> UInt32_T {
        VkPhysicalDeviceMemoryProperties memProperties{};
        vkGetPhysicalDeviceMemoryProperties(m_PhysicalDevice, &memProperties);
        for (UInt32_T i{}; i < memProperties.memoryTypeCount; i++) {
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
                return i;
        }

        throw std::runtime_error("failed to find suitable memory type!");
    }

    auto VulkanRenderer::BeginSingleTimeCommands() -> VkCommandBuffer {
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = m_CommandPool;
        allocInfo.commandBufferCount = 1;

        VkCommandBuffer commandBuffer{};
        vkAllocateCommandBuffers(m_Device, &allocInfo, &commandBuffer);

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(commandBuffer, &beginInfo);
        return commandBuffer;
    }

    auto VulkanRenderer::EndSingleTimeCommands(VkCommandBuffer commandBuffer) -> void {
        vkEndCommandBuffer(commandBuffer);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        vkQueueSubmit(m_GraphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(m_GraphicsQueue);

        vkFreeCommandBuffers(m_Device, m_CommandPool, 1, &commandBuffer);
    }

    auto VulkanRenderer::CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) -> void {
        VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

        VkBufferCopy copyRegion{};
        copyRegion.srcOffset = 0;  // Optional
        copyRegion.dstOffset = 0;  // Optional
        copyRegion.size = size;
        vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

        EndSingleTimeCommands(commandBuffer);
    }

    auto VulkanRenderer::CopyBufferToImage(VkBuffer buffer, VkImage image, UInt32_T width, UInt32_T height, UInt32_T layerCount) -> void {
        VkCommandBuffer commandBuffer{ BeginSingleTimeCommands() };

        VkBufferImageCopy region{};
        region.bufferOffset = 0;
        region.bufferRowLength = 0;
        region.bufferImageHeight = 0;

        region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        region.imageSubresource.mipLevel = 0;
        region.imageSubresource.baseArrayLayer = 0;
        region.imageSubresource.layerCount = layerCount;

        region.imageOffset = { 0, 0, 0 };
        region.imageExtent = { width, height, 1 };

        vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
        EndSingleTimeCommands(commandBuffer);
    }

    auto VulkanRenderer::CreateImageWithInfo(const VkImageCreateInfo &imageInfo, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory) -> void {
        if (vkCreateImage(m_Device, &imageInfo, nullptr, &image) != VK_SUCCESS)
            throw std::runtime_error("failed to create image!");

        VkMemoryRequirements memRequirements{};
        vkGetImageMemoryRequirements(m_Device, image, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties);

        if (vkAllocateMemory(m_Device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS)
            throw std::runtime_error("failed to allocate image memory!");

        if (vkBindImageMemory(m_Device, image, imageMemory, 0) != VK_SUCCESS)
            throw std::runtime_error("failed to bind image memory!");
    }


    auto VulkanRenderer::AcquireNextImage(uint32_t* imageIndex) -> VkResult {
        vkWaitForFences(m_Device, 1, &m_InFlightFences[m_CurrentFrame], VK_TRUE, std::numeric_limits<UInt64_T>::max());
        VkResult result{ vkAcquireNextImageKHR( m_Device, m_SwapChain, std::numeric_limits<UInt32_T>::max(), m_ImageAvailableSemaphores[m_CurrentFrame],  /* must be a not signaled semaphore*/ VK_NULL_HANDLE, imageIndex) };
        return result;
    }

    auto VulkanRenderer::SubmitCommandBuffers(const VkCommandBuffer* buffers, const UInt32_T* imageIndex) -> VkResult {
        if (m_ImagesInFlight[*imageIndex] != VK_NULL_HANDLE)
            vkWaitForFences(m_Device, 1, &m_ImagesInFlight[*imageIndex], VK_TRUE, UINT64_MAX);

        m_ImagesInFlight[*imageIndex] = m_InFlightFences[m_CurrentFrame];

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        std::array<VkSemaphore, 1> waitSemaphores{ m_ImageAvailableSemaphores[m_CurrentFrame] };
        std::array<VkPipelineStageFlags, 1> waitStages{ VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.waitSemaphoreCount = waitSemaphores.size();
        submitInfo.pWaitSemaphores = waitSemaphores.data();
        submitInfo.pWaitDstStageMask = waitStages.data();

        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = buffers;

        std::array<VkSemaphore, 1> signalSemaphores{ m_RenderFinishedSemaphores[m_CurrentFrame] };
        submitInfo.signalSemaphoreCount = signalSemaphores.size();
        submitInfo.pSignalSemaphores = signalSemaphores.data();

        vkResetFences(m_Device, 1, &m_InFlightFences[m_CurrentFrame]);
        if (vkQueueSubmit(GetGraphicsQueue(), 1, &submitInfo, m_InFlightFences[m_CurrentFrame]) != VK_SUCCESS)
            throw std::runtime_error("failed to submit draw command buffer!");


        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = signalSemaphores.size();
        presentInfo.pWaitSemaphores = signalSemaphores.data();

        std::array<VkSwapchainKHR, 1> swapChains{ m_SwapChain };
        presentInfo.swapchainCount = swapChains.size();
        presentInfo.pSwapchains = swapChains.data();
        presentInfo.pImageIndices = imageIndex;

        auto result{ vkQueuePresentKHR(GetPresentQueue(), &presentInfo) };
        m_CurrentFrame = (m_CurrentFrame + 1) % MAX_FRAMES_IN_FLIGHT;

        return result;
    }

    auto VulkanRenderer::CreateSwapChain() -> void {
        SwapChainSupportDetails swapChainSupport{ GetSwapChainSupport() };

        VkSurfaceFormatKHR surfaceFormat{ ChooseSwapSurfaceFormat(swapChainSupport.Formats) };
        VkPresentModeKHR presentMode{ ChooseSwapPresentMode(swapChainSupport.PresentModes) };
        VkExtent2D extent{ ChooseSwapExtent(swapChainSupport.Capabilities) };

        // We may sometimes have to wait on the driver to complete internal operations
        // before we can acquire another image to render to. Therefore, it is recommended
        // to request at least one more image than the minimum
        const UInt32_T EXTRA_IMAGE_REQUEST{ 1 };

        UInt32_T imageCount{ swapChainSupport.Capabilities.minImageCount + EXTRA_IMAGE_REQUEST };

        if (swapChainSupport.Capabilities.maxImageCount > 0 && imageCount > swapChainSupport.Capabilities.maxImageCount)
            imageCount = swapChainSupport.Capabilities.maxImageCount;


        VkSwapchainCreateInfoKHR createInfo{};

        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = GetSurface();
        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        QueueFamilyIndices indices{ FindPhysicalQueueFamilies() };
        std::array<UInt32_T, 2> queueFamilyIndices{ indices.GraphicsFamily, indices.PresentFamily};

        if (indices.GraphicsFamily != indices.PresentFamily) {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = queueFamilyIndices.size();
            createInfo.pQueueFamilyIndices = queueFamilyIndices.data();
        }
        else {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        }

        createInfo.preTransform = swapChainSupport.Capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;

        createInfo.oldSwapchain = VK_NULL_HANDLE;

        auto result{ vkCreateSwapchainKHR(m_Device, &createInfo, nullptr, &m_SwapChain) };

        if (result != VK_SUCCESS)
            throw std::runtime_error("failed to create swap chain!");

        // We only specified a minimum number of images in the swap chain, so the implementation is
        // allowed to create a swap chain with more. That's why we'll first query the final number of
        // images with vkGetSwapchainImagesKHR, then resize the container and finally call it again to
        // retrieve the handles.
        vkGetSwapchainImagesKHR(m_Device, m_SwapChain, &imageCount, nullptr);
        m_SwapChainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(m_Device, m_SwapChain, &imageCount, m_SwapChainImages.data());

        m_SwapChainImageFormat = surfaceFormat.format;
        m_SwapChainExtent = extent;
    }

    auto VulkanRenderer::CreateImageViews() -> void {
        m_SwapChainImageViews.resize(m_SwapChainImages.size());
        for (std::size_t i{}; i < m_SwapChainImages.size(); i++) {
            VkImageViewCreateInfo viewInfo{};

            viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            viewInfo.image = m_SwapChainImages[i];

            viewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            viewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            viewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            viewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

            viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            viewInfo.subresourceRange.baseMipLevel = 0;
            viewInfo.subresourceRange.levelCount = 1;
            viewInfo.subresourceRange.baseArrayLayer = 0;
            viewInfo.subresourceRange.layerCount = 1;

            viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            viewInfo.format = m_SwapChainImageFormat;

            if (vkCreateImageView(m_Device, &viewInfo, nullptr, &m_SwapChainImageViews[i]) != VK_SUCCESS)
                throw std::runtime_error("failed to create texture image view!");

        }
    }

    auto VulkanRenderer::CreateRenderPass() -> void {
        VkAttachmentDescription depthAttachment{};
        depthAttachment.format = FindDepthFormat();
        depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkAttachmentReference depthAttachmentRef{};
        depthAttachmentRef.attachment = 1;
        depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkAttachmentDescription colorAttachment{};
        colorAttachment.format = GetSwapChainImageFormat();
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference colorAttachmentRef{};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;
        subpass.pDepthStencilAttachment = &depthAttachmentRef;

        VkSubpassDependency dependency{};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

        std::array<VkAttachmentDescription, 2> attachments{ colorAttachment, depthAttachment };

        VkRenderPassCreateInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = static_cast<UInt32_T>(attachments.size());
        renderPassInfo.pAttachments = attachments.data();
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;
        renderPassInfo.dependencyCount = 1;
        renderPassInfo.pDependencies = &dependency;

        if (vkCreateRenderPass(m_Device, &renderPassInfo, nullptr, &m_RenderPass) != VK_SUCCESS)
            throw std::runtime_error("failed to create render pass!");

    }

    auto VulkanRenderer::CreateFrameBuffers() -> void {
        m_SwapChainFrameBuffers.resize(GetImageCount());
        for (std::size_t i{}; i < GetImageCount(); i++) {
            std::array<VkImageView, 2> attachments{ m_SwapChainImageViews[i], m_DepthImageViews[i] };

            VkExtent2D swapChainExtent{ GetSwapChainExtent() };

            VkFramebufferCreateInfo framebufferInfo{};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = m_RenderPass;
            framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
            framebufferInfo.pAttachments = attachments.data();
            framebufferInfo.width = swapChainExtent.width;
            framebufferInfo.height = swapChainExtent.height;
            framebufferInfo.layers = 1;

            if (vkCreateFramebuffer(m_Device, &framebufferInfo, nullptr, &m_SwapChainFrameBuffers[i]) != VK_SUCCESS)
                throw std::runtime_error("failed to create framebuffer!");

        }
    }

    auto VulkanRenderer::CreateDepthResources() -> void {
        VkFormat depthFormat = FindDepthFormat();
        VkExtent2D swapChainExtent = GetSwapChainExtent();

        m_DepthImages.resize(GetImageCount());
        m_DepthImageMemories.resize(GetImageCount());
        m_DepthImageViews.resize(GetImageCount());

        for (std::size_t i{}; i < m_DepthImages.size(); i++) {
            VkImageCreateInfo imageInfo{};
            imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
            imageInfo.imageType = VK_IMAGE_TYPE_2D;
            imageInfo.extent.width = swapChainExtent.width;
            imageInfo.extent.height = swapChainExtent.height;
            imageInfo.extent.depth = 1;
            imageInfo.mipLevels = 1;
            imageInfo.arrayLayers = 1;
            imageInfo.format = depthFormat;
            imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
            imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            imageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
            imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
            imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
            imageInfo.flags = 0;

            CreateImageWithInfo(
                    imageInfo,
                    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                    m_DepthImages[i],
                    m_DepthImageMemories[i]);

            VkImageViewCreateInfo viewInfo{};
            viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            viewInfo.image = m_DepthImages[i];
            viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            viewInfo.format = depthFormat;
            viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
            viewInfo.subresourceRange.baseMipLevel = 0;
            viewInfo.subresourceRange.levelCount = 1;
            viewInfo.subresourceRange.baseArrayLayer = 0;
            viewInfo.subresourceRange.layerCount = 1;

            if (vkCreateImageView(m_Device, &viewInfo, nullptr, &m_DepthImageViews[i]) != VK_SUCCESS) {
                throw std::runtime_error("failed to create texture image view!");
            }
        }
    }

    auto VulkanRenderer::CreateSyncObjects() -> void {
        m_ImageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        m_RenderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        m_InFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
        m_ImagesInFlight.resize(GetImageCount(), VK_NULL_HANDLE);

        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for (std::size_t i{}; i < MAX_FRAMES_IN_FLIGHT; i++) {
            if (vkCreateSemaphore(m_Device, &semaphoreInfo, nullptr, &m_ImageAvailableSemaphores[i]) != VK_SUCCESS ||
                vkCreateSemaphore(m_Device, &semaphoreInfo, nullptr, &m_RenderFinishedSemaphores[i]) != VK_SUCCESS ||
                vkCreateFence(m_Device, &fenceInfo, nullptr, &m_InFlightFences[i]) != VK_SUCCESS)
            {
                throw std::runtime_error("failed to create synchronization objects for a frame!");
            }
        }
    }

    auto VulkanRenderer::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) -> VkSurfaceFormatKHR {
        for (const auto& availableFormat: availableFormats) {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM &&
                availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                return availableFormat;
            }
        }

        return availableFormats[0];
    }

    auto VulkanRenderer::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes) -> VkPresentModeKHR {
        for (const auto &availablePresentMode: availablePresentModes) {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
                return availablePresentMode;
            }
        }

        return VK_PRESENT_MODE_FIFO_KHR;
    }

    auto VulkanRenderer::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities) -> VkExtent2D {
        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
            return capabilities.currentExtent;
        }
        else {
            VkExtent2D actualExtent{ m_WindowExtent };
            actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
            actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

            return actualExtent;
        }
    }

    auto VulkanRenderer::FindDepthFormat() -> VkFormat {
        return  FindSupportedFormat(
                { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
                VK_IMAGE_TILING_OPTIMAL,
                VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
    }

    auto VulkanRenderer::CreateGraphicsPipeline(const Path_T &vPath, const Path_T &fPath, const PipelineConfigInfo& config) -> void {
        KT_ASSERT(config.pipelineLayout != VK_NULL_HANDLE, "Cannot create graphics pipeline. No Pipeline Layout in PipelineConfigInfo");
        KT_ASSERT(config.renderPass != VK_NULL_HANDLE, "Cannot create graphics pipeline. No Render Pass Layout in PipelineConfigInfo");

        auto vData{ GetFileData(vPath) };
        auto fData{ GetFileData(fPath) };

        KATE_CORE_LOGGER_DEBUG("Loaded vertex shader data. Size {}", vData.size());
        KATE_CORE_LOGGER_DEBUG("Loaded fragment shader data. Size {}", fData.size());

        CreateShaderModule(vData, &m_VertShaderModule);
        CreateShaderModule(fData, &m_FragShaderModule);

        std::array<VkPipelineShaderStageCreateInfo, 2> shaderStages{};

        shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
        shaderStages[0].module = m_VertShaderModule;
        shaderStages[0].pName = "main";
        shaderStages[0].flags = 0;
        shaderStages[0].pNext = nullptr;
        shaderStages[0].pSpecializationInfo = nullptr;

        shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        shaderStages[1].module = m_FragShaderModule;
        shaderStages[1].pName = "main";
        shaderStages[1].flags = 0;
        shaderStages[1].pNext = nullptr;
        shaderStages[1].pSpecializationInfo = nullptr;


        // Setup Vertex Input State Info
        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

        auto bindingDesc{ VulkanVertexBuffer::GetDefaultBindingDescriptions() };
        auto attributeDesc{ VulkanVertexBuffer::GetDefaultAttributeDescriptions() };

        vertexInputInfo.vertexBindingDescriptionCount = bindingDesc.size();
        vertexInputInfo.vertexAttributeDescriptionCount = attributeDesc.size();

        vertexInputInfo.pVertexAttributeDescriptions = attributeDesc.data();
        vertexInputInfo.pVertexBindingDescriptions = bindingDesc.data();

        VkGraphicsPipelineCreateInfo pipelineInfo{};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = 2;
        pipelineInfo.pStages =  shaderStages.data();
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &config.inputAssemblyInfo;
        pipelineInfo.pViewportState = &config.ViewportInfo;
        pipelineInfo.pRasterizationState = &config.rasterizationInfo;
        pipelineInfo.pMultisampleState = &config.multisampleInfo;
        pipelineInfo.pColorBlendState = &config.colorBlendInfo;
        pipelineInfo.pDepthStencilState = &config.depthStencilInfo;
        pipelineInfo.layout = config.pipelineLayout;
        pipelineInfo.renderPass = config.renderPass;
        pipelineInfo.subpass = config.subpass;
        pipelineInfo.basePipelineIndex = -1;
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
        pipelineInfo.pDynamicState = &config.DynamicStateInfo;

        if (vkCreateGraphicsPipelines(m_Device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_GraphicsPipeline) != VK_SUCCESS)
            throw std::runtime_error("Failed to create Graphics pipeline");
    }

    auto VulkanRenderer::CreateShaderModule(const CharArray& srcCode, VkShaderModule* shaderModule) -> void {
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = srcCode.size();
        // It seems this casts is valid since the default std::vector allocator
        // ensures the data satisfies the worst case alignment
        createInfo.pCode = reinterpret_cast<const UInt32_T*>(srcCode.data());

        if (vkCreateShaderModule(m_Device, &createInfo, nullptr, shaderModule) != VK_SUCCESS)
            throw std::runtime_error("Failed to create shader module");
    }


    auto VulkanRenderer::GetDefaultPipelineConfigInfo() -> PipelineConfigInfo {
        PipelineConfigInfo configInfo{};

        configInfo.inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        // Every three vertices are group together into a separate triangle
        configInfo.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        configInfo.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

        configInfo.ViewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        configInfo.ViewportInfo.viewportCount = 1;
        configInfo.ViewportInfo.pViewports = nullptr;
        configInfo.ViewportInfo.scissorCount = 1;
        configInfo.ViewportInfo.pScissors = nullptr;


        constexpr float GPU_STANDARD_LINE_WIDTH{ 1.0f };
        configInfo.rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        configInfo.rasterizationInfo.depthClampEnable = VK_FALSE;
        configInfo.rasterizationInfo.rasterizerDiscardEnable = VK_FALSE; // requires extension
        configInfo.rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
        configInfo.rasterizationInfo.lineWidth = configInfo.rasterizationInfo.polygonMode == VK_POLYGON_MODE_LINE ? GPU_STANDARD_LINE_WIDTH : 0.0f; // The maximum line width that is supported depends on the hardware, and any line thicker than 1.0f requires you to enable the wideLines GPU feature.
        configInfo.rasterizationInfo.cullMode = VK_CULL_MODE_BACK_BIT;
        configInfo.rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
        configInfo.rasterizationInfo.depthBiasEnable = VK_FALSE;
        configInfo.rasterizationInfo.depthBiasConstantFactor = 0.0f;
        configInfo.rasterizationInfo.depthBiasClamp = 0.0f;
        configInfo.rasterizationInfo.depthBiasSlopeFactor = 0.0f;


        configInfo.multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        configInfo.multisampleInfo.sampleShadingEnable = VK_FALSE;
        configInfo.multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        configInfo.multisampleInfo.minSampleShading = 1.0f;             // Optional
        configInfo.multisampleInfo.pSampleMask = nullptr;               // Optional
        configInfo.multisampleInfo.alphaToCoverageEnable = VK_FALSE;    // Optional
        configInfo.multisampleInfo.alphaToOneEnable = VK_FALSE;         // Optional

        // Blending enabled by default
        configInfo.colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        configInfo.colorBlendAttachment.blendEnable = VK_TRUE;
        configInfo.colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
        configInfo.colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        configInfo.colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
        configInfo.colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        configInfo.colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        configInfo.colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

        configInfo.colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        configInfo.colorBlendInfo.logicOpEnable = VK_FALSE;
        configInfo.colorBlendInfo.logicOp = VK_LOGIC_OP_COPY;
        configInfo.colorBlendInfo.attachmentCount = 1;
        configInfo.colorBlendInfo.pAttachments = &configInfo.colorBlendAttachment;
        configInfo.colorBlendInfo.blendConstants[0] = 0.0f;
        configInfo.colorBlendInfo.blendConstants[1] = 0.0f;
        configInfo.colorBlendInfo.blendConstants[2] = 0.0f;
        configInfo.colorBlendInfo.blendConstants[3] = 0.0f;

        configInfo.depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        configInfo.depthStencilInfo.depthTestEnable = VK_TRUE;
        configInfo.depthStencilInfo.depthWriteEnable = VK_TRUE;
        configInfo.depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
        configInfo.depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
        configInfo.depthStencilInfo.minDepthBounds = 0.0f;  // Optional
        configInfo.depthStencilInfo.maxDepthBounds = 1.0f;  // Optional
        configInfo.depthStencilInfo.stencilTestEnable = VK_FALSE;
        configInfo.depthStencilInfo.front = {};  // Optional
        configInfo.depthStencilInfo.back = {};   // Optional

        configInfo.DynamicStateEnables = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR, VK_DYNAMIC_STATE_LINE_WIDTH };
        configInfo.DynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        configInfo.DynamicStateInfo.pDynamicStates = configInfo.DynamicStateEnables.data();
        configInfo.DynamicStateInfo.dynamicStateCount = configInfo.DynamicStateEnables.size();
        configInfo.DynamicStateInfo.flags = 0;


        return configInfo;
    }

    auto VulkanRenderer::Bind(VkCommandBuffer commandBuffer) -> void {
        // Not necessary, but just in case
        KT_ASSERT(m_GraphicsPipeline, "Graphics pipeline is NULL");

        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_GraphicsPipeline);
    }
}
