//
// Created by kate on 7/10/2023.
//

#include <volk.h>

#include <Renderer/Vulkan/VulkanContext.hh>
#include <Renderer/Vulkan/VulkanFrameBuffer.hh>

namespace kaTe {
    VulkanFrameBuffer::VulkanFrameBuffer(const FrameBufferCreateInfo &createInfo)
        :   m_CreateInfo{ createInfo }
    {
        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = VK_NULL_HANDLE;
        framebufferInfo.attachmentCount = 0;
        framebufferInfo.pAttachments = nullptr;
        framebufferInfo.width = m_CreateInfo.width;
        framebufferInfo.height = m_CreateInfo.height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(VulkanContext::GetPrimaryLogicalDevice(), &framebufferInfo, nullptr, &m_FrameBuffer) != VK_SUCCESS)
            throw std::runtime_error("failed to create framebuffer!");
    }

    auto VulkanFrameBuffer::GetId() -> kaTe::UInt32_T {
        return 0;
    }

    auto VulkanFrameBuffer::GetFrameBufferProperties() const -> const FrameBufferCreateInfo& {
        return m_CreateInfo;
    }

    auto kaTe::VulkanFrameBuffer::GetColorAttachmentId() -> kaTe::UInt32_T {
        return 0;
    }

    auto VulkanFrameBuffer::GetDepthAttachmentId() -> kaTe::UInt32_T {
        return 0;
    }

    auto VulkanFrameBuffer::OnRelease() const -> void {
        vkDestroyFramebuffer(VulkanContext::GetPrimaryLogicalDevice(), m_FrameBuffer, nullptr);
    }

}