/**
 * VulkanCommandPool.hh
 * Created by kate on 7/4/2023.
 * */

#ifndef KATE_ENGINE_VULKAN_COMMAND_POOL_HH
#define KATE_ENGINE_VULKAN_COMMAND_POOL_HH

// Third-Party Libraries
#include <volk.h>

// Project Headers
#include <Tools/Common.hh>

namespace kaTe {
    class VulkanCommandPool {
    public:
        explicit VulkanCommandPool() = default;
        ~VulkanCommandPool() = default;

        auto OnCreate() -> void;

        KT_NODISCARD auto GetCommandPool() const -> VkCommandPool { return m_CommandPool; }
        KT_NODISCARD auto BeginSingleTimeCommands() const -> VkCommandBuffer;
        auto EndSingleTimeCommands(VkCommandBuffer commandBuffer) const -> void;

        auto OnRelease() const -> void;
    private:
        VkCommandPool  m_CommandPool{};
    };
}


#endif //KATE_ENGINE_VULKAN_COMMAND_POOL_HH
