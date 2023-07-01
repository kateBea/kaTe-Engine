//
// Created by kate on 6/2/23.
//

#ifndef KATE_ENGINE_VULKAN_DEVICE_HH
#define KATE_ENGINE_VULKAN_DEVICE_HH

#include <memory>
#include <string>
#include <vector>

#include <volk.h>

#include <Tools/Common.hh>
#include <Platform/Window/MainWindow.hh>

namespace kaTe {

    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR        Capabilities{};
        std::vector<VkSurfaceFormatKHR> Formats{};
        std::vector<VkPresentModeKHR>   PresentModes{};
    };

    struct QueueFamilyIndices {
        UInt32_T    GraphicsFamily{};
        UInt32_T    PresentFamily{};

        // TODO: use std::optional
        bool GraphicsFamilyHasValue{ false };
        bool PresentFamilyHasValue{ false };

        KT_NODISCARD auto IsComplete() const -> bool { return GraphicsFamilyHasValue && PresentFamilyHasValue; }
    };

    class VulkanDevice {
    public:
        explicit VulkanDevice(std::shared_ptr<MainWindow> window);

        KT_NODISCARD auto GetCommandPool() -> VkCommandPool { return m_CommandPool; }
        KT_NODISCARD auto GetDevice() -> VkDevice { return m_Device; }
        KT_NODISCARD auto GetSurface() -> VkSurfaceKHR { return m_Surface; }
        KT_NODISCARD auto GetGraphicsQueue() -> VkQueue { return m_GraphicsQueue; }
        KT_NODISCARD auto GetPresentQueue() -> VkQueue { return m_PresentQueue; }
        KT_NODISCARD auto GetSwapChainSupport() -> SwapChainSupportDetails { return QuerySwapChainSupport(m_PhysicalDevice); }
        KT_NODISCARD auto FindMemoryType(UInt32_T typeFilter, VkMemoryPropertyFlags properties) -> UInt32_T;
        KT_NODISCARD auto FindPhysicalQueueFamilies() -> QueueFamilyIndices { return FindQueueFamilies(m_PhysicalDevice); }
        KT_NODISCARD auto FindSupportedFormat(const std::vector<VkFormat> &candidates, VkImageTiling tiling, VkFormatFeatureFlags features) -> VkFormat;

        KT_NODISCARD auto BeginSingleTimeCommands() -> VkCommandBuffer;

        auto EndSingleTimeCommands(VkCommandBuffer commandBuffer) -> void;
        auto CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) -> void;
        auto CopyBufferToImage(VkBuffer buffer, VkImage image, UInt32_T width, UInt32_T height, UInt32_T layerCount) -> void;
        auto CreateImageWithInfo(const VkImageCreateInfo &imageInfo, VkMemoryPropertyFlags properties, VkImage &image, VkDeviceMemory& imageMemory) -> void;

        auto OnDestroy() -> void;

        ~VulkanDevice() = default;
    private:
        // VALIDATION LAYERS. Enabled on Debug builds
#if defined(NDEBUG)
        static constexpr bool m_EnableValidationLayers{ false };
#else
        static constexpr bool m_EnableValidationLayers{ true };
#endif
    public:
        // Forbidden operations
        VulkanDevice(const VulkanDevice&) = delete;
        auto operator=(const VulkanDevice&) -> VulkanDevice& = delete;

        VulkanDevice(VulkanDevice &&) = delete;
        VulkanDevice &operator=(VulkanDevice&&) = delete;
    private:
        auto CreateInstance() -> void;
        auto SetupDebugMessenger() -> void;
        auto CreateSurface() -> void;
        auto PickPhysicalDevice() -> void;
        auto CreateLogicalDevice() -> void;
        auto CreateCommandPool() -> void;

        // helper functions
        KT_NODISCARD auto IsDeviceSuitable(VkPhysicalDevice device) -> bool;
        KT_NODISCARD auto GetRequiredExtensions() const -> std::vector<const char*>;
        KT_NODISCARD auto FindQueueFamilies(VkPhysicalDevice device) -> QueueFamilyIndices;
        KT_NODISCARD auto CheckDeviceExtensionSupport(VkPhysicalDevice device) -> bool;

        auto CheckValidationLayerSupport() -> bool;
        auto PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) -> void;
        auto HasGflwRequiredInstanceExtensions() -> void;


        KT_NODISCARD auto QuerySwapChainSupport(VkPhysicalDevice device) -> SwapChainSupportDetails;
    private:
        VkInstance                      m_Instance{};
        VkDebugUtilsMessengerEXT        m_DebugMessenger{};
        VkPhysicalDevice                m_PhysicalDevice{};
        std::shared_ptr<MainWindow>     m_Window{};
        VkCommandPool                   m_CommandPool{};

        VkPhysicalDeviceProperties      m_Properties{};

        VkDevice                        m_Device{};
        VkSurfaceKHR                    m_Surface{};
        VkQueue                         m_GraphicsQueue{};
        VkQueue                         m_PresentQueue{};

        const std::vector<const char *> m_ValidationLayers{ "VK_LAYER_KHRONOS_validation" };
        const std::vector<const char *> m_DeviceRequiredExtensions{
                VK_KHR_SWAPCHAIN_EXTENSION_NAME,
                // VK_EXT_VERTEX_INPUT_DYNAMIC_STATE_EXTENSION_NAME,
                // VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME // not supported by the device this project was tested on but is required by VK_EXT_VERTEX_INPUT_DYNAMIC_STATE_EXTENSION_NAME
                // TODO: Implement static vertex buffer when the necessary extensions are not supported
        };
    };

}

#endif // KATE_ENGINE_VULKAN_DEVICE_HH