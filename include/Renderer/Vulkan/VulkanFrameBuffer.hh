//
// Created by kate on 7/10/2023.
//

#ifndef KATE_ENGINE_VULKAN_FRAMEBUFFER_HH
#define KATE_ENGINE_VULKAN_FRAMEBUFFER_HH

#include <volk.h>

#include <Renderer/Buffers/FrameBuffer.hh>

namespace kaTe {
    class VulkanFrameBuffer : public FrameBuffer {
    public:
        explicit VulkanFrameBuffer(const FrameBufferCreateInfo& createInfo);
        KT_NODISCARD auto GetId() -> UInt32_T override;
        KT_NODISCARD auto GetFrameBufferProperties() const -> const FrameBufferCreateInfo& override;

        KT_NODISCARD auto GetColorAttachmentId() -> UInt32_T override;
        KT_NODISCARD auto GetDepthAttachmentId() -> UInt32_T override;

        auto OnRelease() const -> void;

    private:
        FrameBufferCreateInfo m_CreateInfo{};
        VkFramebuffer m_FrameBuffer{};
    };
}


#endif //KATE_ENGINE_VULKAN_FRAMEBUFFER_HH
