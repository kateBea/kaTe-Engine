#include <set>
#include <cstring>
#include <unordered_set>
#include <memory>
#include <utility>

#include <Tools/Common.hh>
#include <Core/Logger.hh>

#include <Renderer/Vulkan/VulkanDevice.hh>
#include <Platform/Window/MainWindow.hh>

namespace kaTe {
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

    auto DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks *pAllocator) -> void {
        auto func{ (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT") };

        if (func != nullptr)
            func(instance, debugMessenger, pAllocator);
    }

    VulkanDevice::VulkanDevice(std::shared_ptr<MainWindow> window)
        :   m_Window{ std::move(window) }
    {
        auto ret{ volkInitialize() };
        KT_ASSERT(ret == VK_SUCCESS, "Failed to init Volk");

        CreateInstance();
        SetupDebugMessenger();
        CreateSurface();
        PickPhysicalDevice();
        CreateLogicalDevice();
        CreateCommandPool();
    }

    auto VulkanDevice::CreateInstance() -> void {
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

    auto VulkanDevice::PickPhysicalDevice() -> void{
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

    auto VulkanDevice::CreateLogicalDevice() -> void {
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

    auto VulkanDevice::CreateCommandPool() -> void {
        QueueFamilyIndices queueFamilyIndices{ FindPhysicalQueueFamilies() };

        VkCommandPoolCreateInfo poolInfo{};

        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        poolInfo.queueFamilyIndex = queueFamilyIndices.GraphicsFamily;

        if (vkCreateCommandPool(m_Device, &poolInfo, nullptr, &m_CommandPool) != VK_SUCCESS)
            throw std::runtime_error("failed to create command pool!");
    }

    auto VulkanDevice::CreateSurface() -> void {
        m_Window->CreateWindowSurface(m_Instance, &m_Surface);
    }

    auto VulkanDevice::IsDeviceSuitable(VkPhysicalDevice device) -> bool {
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

    auto VulkanDevice::PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) -> void {
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

    auto VulkanDevice::SetupDebugMessenger() -> void {
        if (!m_EnableValidationLayers)
            return;

        VkDebugUtilsMessengerCreateInfoEXT createInfo{};
        PopulateDebugMessengerCreateInfo(createInfo);
        if (CreateDebugUtilsMessengerEXT(m_Instance, &createInfo, nullptr, &m_DebugMessenger) != VK_SUCCESS)
            throw std::runtime_error("failed to set up debug messenger!");
    }

    auto VulkanDevice::CheckValidationLayerSupport() -> bool {
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

    auto VulkanDevice::GetRequiredExtensions() const -> std::vector<const char*> {
        UInt32_T glfwExtensionCount{};
        const char** glfwExtensions{ glfwGetRequiredInstanceExtensions(&glfwExtensionCount) };
        std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

        if (m_EnableValidationLayers)
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

        return extensions;
    }

    auto VulkanDevice::HasGflwRequiredInstanceExtensions() -> void {
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

    auto VulkanDevice::CheckDeviceExtensionSupport(VkPhysicalDevice device) -> bool {
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

    auto VulkanDevice::FindQueueFamilies(VkPhysicalDevice device) -> QueueFamilyIndices {
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

    auto VulkanDevice::QuerySwapChainSupport(VkPhysicalDevice device) -> SwapChainSupportDetails {
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

    auto VulkanDevice::FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) -> VkFormat {
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

    auto VulkanDevice::FindMemoryType(UInt32_T typeFilter, VkMemoryPropertyFlags properties) -> UInt32_T {
        VkPhysicalDeviceMemoryProperties memProperties{};
        vkGetPhysicalDeviceMemoryProperties(m_PhysicalDevice, &memProperties);
        for (UInt32_T i{}; i < memProperties.memoryTypeCount; i++) {
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
                return i;
        }

        throw std::runtime_error("failed to find suitable memory type!");
    }

    auto VulkanDevice::BeginSingleTimeCommands() -> VkCommandBuffer {
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

    auto VulkanDevice::EndSingleTimeCommands(VkCommandBuffer commandBuffer) -> void {
        vkEndCommandBuffer(commandBuffer);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        vkQueueSubmit(m_GraphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(m_GraphicsQueue);

        vkFreeCommandBuffers(m_Device, m_CommandPool, 1, &commandBuffer);
    }

    auto VulkanDevice::CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) -> void {
        VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

        VkBufferCopy copyRegion{};
        copyRegion.srcOffset = 0;  // Optional
        copyRegion.dstOffset = 0;  // Optional
        copyRegion.size = size;
        vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

        EndSingleTimeCommands(commandBuffer);
    }

    auto VulkanDevice::CopyBufferToImage(VkBuffer buffer, VkImage image, UInt32_T width, UInt32_T height, UInt32_T layerCount) -> void {
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

    auto VulkanDevice::CreateImageWithInfo(const VkImageCreateInfo &imageInfo, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory) -> void {
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

    auto VulkanDevice::OnDestroy() -> void {
        vkDestroyCommandPool(m_Device, m_CommandPool, nullptr);
        vkDestroyDevice(m_Device, nullptr);

        if (m_EnableValidationLayers)
            DestroyDebugUtilsMessengerEXT(m_Instance, m_DebugMessenger, nullptr);

        vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);
        vkDestroyInstance(m_Instance, nullptr);
    }
}