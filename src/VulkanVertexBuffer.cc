//
// Created by kate on 6/17/23.
//

#include <array>
#include <memory>
#include <utility>
#include <stdexcept>
#include <cstring>

#include <Core/Assert.hh>
#include <Core/Logger.hh>

#include <Renderer/Vulkan/VulkanContext.hh>
#include <Renderer/Vulkan/VulkanRenderer.hh>
#include <Renderer/Vulkan/VulkanVertexBuffer.hh>

namespace kaTe {
    VulkanVertexBuffer::VulkanVertexBuffer(const std::vector<float>& data, const BufferLayout& layout)
        :   m_Layout{ layout }
    {
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
        // All of our per-vertex data is packed together in one array, so we're only going to have one binding.
        // See: https://vulkan-tutorial.com/Vertex_buffers/Vertex_input_description

        s_BindingDesc = std::vector<VkVertexInputBindingDescription>(1);

        s_BindingDesc[0] = {};
        s_BindingDesc[0].binding = 0;
        s_BindingDesc[0].stride = s_Layout.GetStride();
        s_BindingDesc[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX; // not using instanced rendering, so we'll stick to per-vertex data.

        return s_BindingDesc;
    }

    auto VulkanVertexBuffer::GetDefaultAttributeDescriptions() -> std::vector<VkVertexInputAttributeDescription>& {
        s_AttributeDesc = std::vector<VkVertexInputAttributeDescription>(s_Layout.GetCount());

        /**
         * The binding parameter tells Vulkan from which binding the per-vertex data comes.
         * The location parameter references the location directive of the input in the vertex shader.
         * The input in the vertex shader with location 0 is the position, which has two 32-bit float
         * components. The format parameter describes the type of data for the attribute
         *
         * See: https://vulkan-tutorial.com/Vertex_buffers/Vertex_input_description
         * */

        /**
         * TODO: add these to the vertex buffer helper functions. for instance if the ShaderDataType is FLOAT3_TYPE return VK_FORMAT_R32G32B32_SFLOAT, and so on
         * NOTE:: already implemented in the kaTe which is private currently engine
         *
         * float: VK_FORMAT_R32_SFLOAT
         * vec2: VK_FORMAT_R32G32_SFLOAT
         * vec3: VK_FORMAT_R32G32B32_SFLOAT
         * vec4: VK_FORMAT_R32G32B32A32_SFLOAT
         *
         * ivec2: VK_FORMAT_R32G32_SINT, a 2-component vector of 32-bit signed integers
         * uvec4: VK_FORMAT_R32G32B32A32_UINT, a 4-component vector of 32-bit unsigned integers
         * double: VK_FORMAT_R64_SFLOAT, a double-precision (64-bit) float
         *
         * */
        auto staticLayout{ s_Layout };

        s_AttributeDesc[0] = {};
        s_AttributeDesc[0].binding = 0;
        s_AttributeDesc[0].location = 0;
        s_AttributeDesc[0].format = VK_FORMAT_R32G32B32_SFLOAT; // TODO: GetVkAttributeType(s_Layout[index].GetType()) // this way i dont have to hardcode the type here
        s_AttributeDesc[0].offset = s_Layout[0].GetOffset();

        s_AttributeDesc[1] = {};
        s_AttributeDesc[1].binding = 0;
        s_AttributeDesc[1].location = 1;
        s_AttributeDesc[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        s_AttributeDesc[1].offset = s_Layout[1].GetOffset();

        s_AttributeDesc[2] = {};
        s_AttributeDesc[2].binding = 0;
        s_AttributeDesc[2].location = 2;
        s_AttributeDesc[2].format = VK_FORMAT_R32G32_SFLOAT;
        s_AttributeDesc[2].offset = s_Layout[2].GetOffset();

        return s_AttributeDesc;
    }

    auto VulkanVertexBuffer::SetVertexData(const std::vector<float>& vertices) -> void {
        KT_ASSERT(vertices.size() >= 3, "Vertex buffer requires at least three vertices");
        m_VertexCount = vertices.size();

        // TODO: use staging buffers
        VkDeviceSize bufferSize{ m_VertexCount * sizeof(float) };

        // Setup staging buffer
        VkBuffer stagingBuffer{};
        VkDeviceMemory stagingBufferMemory{};
        CreateBuffer(bufferSize,
                     VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                     VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                     stagingBuffer, stagingBufferMemory);

        void* data{};
        if (vkMapMemory(VulkanContext::GetPrimaryLogicalDevice(), stagingBufferMemory, 0, bufferSize, 0, &data) != VK_SUCCESS)
            throw std::runtime_error("Failed to map memory");
        std::memcpy(data, static_cast<const void*>(vertices.data()), static_cast<std::size_t>(bufferSize));
        vkUnmapMemory(VulkanContext::GetPrimaryLogicalDevice(), stagingBufferMemory);

        // Setup vertex buffer
        CreateBuffer(
                bufferSize,
                VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_VertexBuffer, m_VertexBufferMemory);

        CopyBuffer(stagingBuffer, m_VertexBuffer, bufferSize);

        vkDestroyBuffer(VulkanContext::GetPrimaryLogicalDevice(), stagingBuffer, nullptr);
        vkFreeMemory(VulkanContext::GetPrimaryLogicalDevice(), stagingBufferMemory, nullptr);
    }

    auto VulkanVertexBuffer::CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) -> void {
        VkCommandBuffer commandBuffer{ dynamic_cast<VulkanRenderer*>(Renderer::GetRendererAPIActive())->GetCommandPool().BeginSingleTimeCommands() };

        VkBufferCopy copyRegion{};
        copyRegion.size = size;
        vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

        dynamic_cast<VulkanRenderer*>(Renderer::GetRendererAPIActive())->GetCommandPool().EndSingleTimeCommands(commandBuffer);
    }

    auto VulkanVertexBuffer::CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) -> void {
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = usage;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(VulkanContext::GetPrimaryLogicalDevice(), &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
            throw std::runtime_error("failed to create vertex buffer!");

        VkMemoryRequirements memRequirements{};
        vkGetBufferMemoryRequirements(VulkanContext::GetPrimaryLogicalDevice(), buffer, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = VulkanContext::FindMemoryType(memRequirements.memoryTypeBits, properties, VulkanContext::GetPrimaryPhysicalDevice());

        /**
         * NOTE:
         * It should be noted that in a real world application, you're not supposed to actually call
         * vkAllocateMemory for every individual buffer. The maximum number of simultaneous memory
         * allocations is limited by the maxMemoryAllocationCount physical device limit, which may
         * be as low as 4096 even on high end hardware like an NVIDIA GTX 1080
         * See: https://vulkan-tutorial.com/Vertex_buffers/Staging_buffer
         * */
        if (vkAllocateMemory(VulkanContext::GetPrimaryLogicalDevice(), &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
            throw std::runtime_error("failed to allocate vertex buffer memory!");

        vkBindBufferMemory(VulkanContext::GetPrimaryLogicalDevice(), buffer, bufferMemory, 0);
    }

    auto VulkanVertexBuffer::OnRelease() -> void {
        vkDestroyBuffer(VulkanContext::GetPrimaryLogicalDevice(), m_VertexBuffer, nullptr);
        vkFreeMemory(VulkanContext::GetPrimaryLogicalDevice(), m_VertexBufferMemory, nullptr);
    }
}