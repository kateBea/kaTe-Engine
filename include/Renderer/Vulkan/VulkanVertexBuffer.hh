//
// Created by kate on 6/17/23.
//

#ifndef KATE_ENGINE_VULKAN_VERTEX_BUFFER_HH
#define KATE_ENGINE_VULKAN_VERTEX_BUFFER_HH

#include <memory>
#include <vector>

#include <glm/glm.hpp>

#include <volk.h>

#include <Tools/Common.hh>

#include <Renderer/Buffers/VertexBuffer.hh>

namespace kaTe {
    class VulkanVertexBuffer : public VertexBuffer {
    public:
        explicit VulkanVertexBuffer(const std::vector<float> &data, const BufferLayout &layout = s_Layout);

        auto Bind(VkCommandBuffer commandBuffer) const -> void;

        auto Bind() const -> void override {}
        auto Unbind() const -> void override {}

        auto SetBufferLayout(const BufferLayout& layout) -> void override { m_Layout = layout; }
        KT_NODISCARD auto GetBufferLayout() const -> const BufferLayout& override { return GetDefaultBufferLayout(); }
        KT_NODISCARD auto GetVertexCount() const -> ULongLong_T { return m_VertexCount; }

        auto GetBindingDescriptions() -> std::vector<VkVertexInputBindingDescription>& { return m_BindingDesc; }
        auto GetAttributeDescriptions() -> std::vector<VkVertexInputAttributeDescription>& { return m_AttributeDesc; }

        static auto GetDefaultBindingDescriptions() -> std::vector<VkVertexInputBindingDescription>&;
        static auto GetDefaultAttributeDescriptions() -> std::vector<VkVertexInputAttributeDescription>&;

        auto OnRelease() -> void override;

        ~VulkanVertexBuffer() override = default;
    public:
        auto SetBindingDescriptions() -> void;
        auto SetAttributeDescriptions() -> void;

        VulkanVertexBuffer(const VulkanVertexBuffer&) = delete;
        auto operator=(const VulkanVertexBuffer&) -> VulkanVertexBuffer& = delete;

        VulkanVertexBuffer(VulkanVertexBuffer&&) = delete;
        auto operator=(VulkanVertexBuffer&&) -> VulkanVertexBuffer& = delete;
    private:
        auto SetVertexData(const std::vector<float>& vertices) -> void;
        auto CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) -> void;
    private:
        inline static std::vector<VkVertexInputBindingDescription>    s_BindingDesc{};
        inline static std::vector<VkVertexInputAttributeDescription>  s_AttributeDesc{};

        static inline BufferLayout s_Layout{
                { ShaderDataType::FLOAT3_TYPE, "a_Position" },
                { ShaderDataType::FLOAT4_TYPE, "a_Color" }
        };

        static auto GetDefaultBufferLayout() -> const BufferLayout& {
            return s_Layout;
        }

        static auto SetDefaultBufferLayout(const BufferLayout& layout) -> void {
            s_Layout = layout;
        }

        std::vector<VkVertexInputBindingDescription> m_BindingDesc{};
        std::vector<VkVertexInputAttributeDescription>  m_AttributeDesc{};
        BufferLayout                    m_Layout{};
        VkBuffer                        m_VertexBuffer{};
        VkDeviceMemory                  m_VertexBufferMemory{};
        ULongLong_T                     m_VertexCount{};
    };
}


#endif // KATE_ENGINE_VULKAN_VERTEX_BUFFER_HH
