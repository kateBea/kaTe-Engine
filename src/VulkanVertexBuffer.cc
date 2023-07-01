//
// Created by kate on 6/17/23.
//

#include <array>
#include <memory>
#include <utility>
#include <stdexcept>
#include <cstring>

#include <volk.h>

#include "Renderer/Renderer.hh"
#include "Renderer/Vulkan/VulkanRenderer.hh"
#include <Core/Assert.hh>
#include <Renderer/Vulkan/VulkanVertexBuffer.hh>

namespace kaTe {
    VulkanVertexBuffer::VulkanVertexBuffer(const std::vector<float> &data, const BufferLayout& layout) {
        VulkanRenderer& renderer{ *dynamic_cast<VulkanRenderer*>(Renderer::GetCurrentRenderer()) };

        m_Layout = layout;
        SetBindingDescriptions();
        SetAttributeDescriptions();

        SetVertexData(data);
    }

    auto VulkanVertexBuffer::Bind(VkCommandBuffer commandBuffer) const -> void {
        std::array<VkBuffer, 1> buffers{ m_VertexBuffer };
        std::array<VkDeviceSize, 1> offsets{ 0 };

        vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers.data(), offsets.data());
    }

    auto VulkanVertexBuffer::SetBindingDescriptions() -> void {

        m_BindingDesc = std::vector<VkVertexInputBindingDescription>(1);
        m_BindingDesc[0].binding = 0;
        m_BindingDesc[0].stride = m_Layout.GetStride();
        m_BindingDesc[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    }

    auto VulkanVertexBuffer::SetAttributeDescriptions() -> void {
        m_AttributeDesc = std::vector<VkVertexInputAttributeDescription>(m_Layout.GetCount());

        m_AttributeDesc[0].binding = 0;
        m_AttributeDesc[0].location = 0;
        m_AttributeDesc[0].format = VK_FORMAT_R32G32_SFLOAT;
        m_AttributeDesc[0].offset = m_Layout[0].GetOffset();

        m_AttributeDesc[1].binding = 0;
        m_AttributeDesc[1].location = 1;
        m_AttributeDesc[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        m_AttributeDesc[1].offset = m_Layout[1].GetOffset();
    }

    auto VulkanVertexBuffer::GetDefaultBindingDescriptions() -> std::vector<VkVertexInputBindingDescription>& {
        s_BindingDesc = std::vector<VkVertexInputBindingDescription>(1);

        s_BindingDesc[0] = {};

        s_BindingDesc[0].binding = 0;
        s_BindingDesc[0].stride = s_Layout.GetStride();
        s_BindingDesc[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return s_BindingDesc;
    }

    auto VulkanVertexBuffer::GetDefaultAttributeDescriptions() -> std::vector<VkVertexInputAttributeDescription>& {
        s_AttributeDesc = std::vector<VkVertexInputAttributeDescription>(s_Layout.GetCount());

        s_AttributeDesc[0] = {};
        s_AttributeDesc[0].binding = 0;
        s_AttributeDesc[0].location = 0;
        s_AttributeDesc[0].format = VK_FORMAT_R32G32_SFLOAT;
        s_AttributeDesc[0].offset = s_Layout[0].GetOffset();

        s_AttributeDesc[1] = {};
        s_AttributeDesc[1].binding = 0;
        s_AttributeDesc[1].location = 1;
        s_AttributeDesc[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        s_AttributeDesc[1].offset = s_Layout[1].GetOffset();

        return s_AttributeDesc;
    }

    auto VulkanVertexBuffer::SetVertexData(const std::vector<float>& vertices) -> void {
        VulkanRenderer& renderer{ *dynamic_cast<VulkanRenderer*>(Renderer::GetCurrentRenderer()) };
        KT_ASSERT(vertices.size() >= 3, "Vertex buffer requires at least three vertices");
        m_VertexCount = vertices.size();

        VkDeviceSize bufferSize{ m_VertexCount * sizeof(float) };
        CreateBuffer(
                bufferSize,
                VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                m_VertexBuffer,
                m_VertexBufferMemory
        );

        void* data{};
        if (vkMapMemory(renderer.m_Device, m_VertexBufferMemory, 0, bufferSize, 0, &data) != VK_SUCCESS)
            throw std::runtime_error("Failed to map memory");

        std::memcpy(data, static_cast<const void*>(vertices.data()), static_cast<std::size_t>(bufferSize));
        vkUnmapMemory(renderer.m_Device, m_VertexBufferMemory);
    }

    auto VulkanVertexBuffer::CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) -> void {
        VulkanRenderer& renderer{ *dynamic_cast<VulkanRenderer*>(Renderer::GetCurrentRenderer()) };

        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = usage;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(renderer.m_Device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
            throw std::runtime_error("failed to create vertex buffer!");

        VkMemoryRequirements memRequirements{};
        vkGetBufferMemoryRequirements(renderer.m_Device, buffer, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = renderer.FindMemoryType(memRequirements.memoryTypeBits, properties);

        if (vkAllocateMemory(renderer.m_Device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
            throw std::runtime_error("failed to allocate vertex buffer memory!");


        vkBindBufferMemory(renderer.m_Device, buffer, bufferMemory, 0);
    }

    VulkanVertexBuffer::~VulkanVertexBuffer() {
        VulkanRenderer& renderer{ *dynamic_cast<VulkanRenderer*>(Renderer::GetCurrentRenderer()) };

        vkDestroyBuffer(renderer.m_Device, m_VertexBuffer, nullptr);
        vkFreeMemory(renderer.m_Device, m_VertexBufferMemory, nullptr);
    }
}