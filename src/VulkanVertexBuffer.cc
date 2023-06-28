//
// Created by kate on 6/17/23.
//

#include <array>
#include <memory>
#include <utility>
#include <stdexcept>
#include <cstring>

#include <volk.h>

#include <Core/Assert.hh>
#include <Renderer/Vulkan/VulkanVertexBuffer.hh>

namespace kaTe {
    VulkanVertexBuffer::VulkanVertexBuffer(std::shared_ptr<VulkanDevice> device, const std::vector<float>& data)
        :   m_Device{ std::move(device) }
    {
        SetVertexData(data);
    }

    auto VulkanVertexBuffer::Draw(VkCommandBuffer commandBuffer) const -> void {
        vkCmdDraw(commandBuffer, m_VertexCount, 1, 0, 0);
    }

    auto VulkanVertexBuffer::Bind(VkCommandBuffer commandBuffer) const -> void {
        std::array<VkBuffer, 1> buffers{ m_VertexBuffer };
        std::array<VkDeviceSize, 1> offsets{ 0 };

        {
            // TODO: temporary
            std::vector<VkVertexInputBindingDescription2EXT> bindingDescriptions(1);

            bindingDescriptions[0].sType = VK_STRUCTURE_TYPE_VERTEX_INPUT_BINDING_DESCRIPTION_2_EXT;
            bindingDescriptions[0].binding = 0;
            bindingDescriptions[0].stride = s_Layout.GetStride();
            bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

            std::vector<VkVertexInputAttributeDescription2EXT> attributeDescriptions(s_Layout.GetCount());

            attributeDescriptions[0].sType = VK_STRUCTURE_TYPE_VERTEX_INPUT_ATTRIBUTE_DESCRIPTION_2_EXT;
            attributeDescriptions[0].binding = 0;
            attributeDescriptions[0].location = 0;
            attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescriptions[0].offset = s_Layout[0].GetOffset();

            attributeDescriptions[1].sType = VK_STRUCTURE_TYPE_VERTEX_INPUT_ATTRIBUTE_DESCRIPTION_2_EXT;
            attributeDescriptions[1].binding = 0;
            attributeDescriptions[1].location = 1;
            attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescriptions[1].offset = s_Layout[1].GetOffset();
            vkCmdSetVertexInputEXT(commandBuffer, bindingDescriptions.size(), bindingDescriptions.data(), attributeDescriptions.size(), attributeDescriptions.data());
        }

        vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers.data(), offsets.data());
    }

    auto VulkanVertexBuffer::SetBindingDescriptions() -> std::vector<VkVertexInputBindingDescription>& {
        m_BindingDesc[0].binding = 0;
        m_BindingDesc[0].stride = s_Layout.GetStride();
        m_BindingDesc[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return m_BindingDesc;
    }

    auto VulkanVertexBuffer::SetAttributeDescriptions() -> std::vector<VkVertexInputAttributeDescription>& {
        m_AttributeDesc = std::vector<VkVertexInputAttributeDescription>(m_Layout.GetCount());

        m_AttributeDesc[0].binding = 0;
        m_AttributeDesc[0].location = 0;
        m_AttributeDesc[0].format = VK_FORMAT_R32G32_SFLOAT;
        m_AttributeDesc[0].offset = m_Layout[0].GetOffset();

        m_AttributeDesc[1].binding = 0;
        m_AttributeDesc[1].location = 1;
        m_AttributeDesc[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        m_AttributeDesc[1].offset = m_Layout[1].GetOffset();

        return m_AttributeDesc;
    }

    auto VulkanVertexBuffer::GetDefaultBindingDescriptions() -> std::vector<VkVertexInputBindingDescription>& {
        s_BindingDesc = std::vector<VkVertexInputBindingDescription>(1);

        s_BindingDesc[0].binding = 0;
        s_BindingDesc[0].stride = s_Layout.GetStride();
        s_BindingDesc[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return s_BindingDesc;
    }

    auto VulkanVertexBuffer::GetDefaultAttributeDescriptions() -> std::vector<VkVertexInputAttributeDescription>& {
        s_AttributeDesc = std::vector<VkVertexInputAttributeDescription>(s_Layout.GetCount());

        s_AttributeDesc[0].binding = 0;
        s_AttributeDesc[0].location = 0;
        s_AttributeDesc[0].format = VK_FORMAT_R32G32_SFLOAT;
        s_AttributeDesc[0].offset = s_Layout[0].GetOffset();

        s_AttributeDesc[1].binding = 0;
        s_AttributeDesc[1].location = 1;
        s_AttributeDesc[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        s_AttributeDesc[1].offset = s_Layout[1].GetOffset();

        return s_AttributeDesc;
    }

    auto VulkanVertexBuffer::SetVertexData(const std::vector<float>& vertices) -> void {
        KT_ASSERT(vertices.size() >= 3, "Vertex buffer requires at least three vertices");
        m_VertexCount = vertices.size();

        VkDeviceSize bufferSize{ m_VertexCount * sizeof(float) };
        m_Device->CreateBuffer(
            bufferSize,
            VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            m_VertexBuffer,
            m_VertexBufferMemory
        );

        void* data{};

        if (vkMapMemory(m_Device->GetDevice(), m_VertexBufferMemory, 0, bufferSize, 0, &data) != VK_SUCCESS)
            throw std::runtime_error("Failed to map memory");

        std::memcpy(data, static_cast<const void*>(vertices.data()), static_cast<std::size_t>(bufferSize));
        vkUnmapMemory(m_Device->GetDevice(), m_VertexBufferMemory);
    }

    VulkanVertexBuffer::~VulkanVertexBuffer() {
        vkDestroyBuffer(m_Device->GetDevice(), m_VertexBuffer, nullptr);
        vkFreeMemory(m_Device->GetDevice(), m_VertexBufferMemory, nullptr);
    }

}