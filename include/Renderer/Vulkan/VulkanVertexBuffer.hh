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

#include <Renderer/Vulkan/VulkanDevice.hh>
#include <Renderer/Buffers/VertexBuffer.hh>

namespace kaTe {
    class VulkanVertexBuffer : public VertexBuffer {
    public:
        explicit VulkanVertexBuffer(std::shared_ptr<VulkanDevice> device, const std::vector<float>& data);

        // TODO: temporary, VertexBuffer base should take any type of param that is necessary for binding the buffer
        auto Bind(VkCommandBuffer commandBuffer) const -> void;
        auto Draw(VkCommandBuffer commandBuffer) const -> void;

        auto Bind() const -> void override {}
        auto Unbind() const -> void override {}

        auto SetBufferLayout(const BufferLayout& layout) -> void override { m_Layout = layout; }
        KT_NODISCARD auto GetBufferLayout() const -> const BufferLayout& override { return GetDefaultBufferLayout(); }

        auto SetBindingDescriptions() -> std::vector<VkVertexInputBindingDescription>&;
        auto SetAttributeDescriptions() -> std::vector<VkVertexInputAttributeDescription>&;

        static auto GetDefaultBindingDescriptions() -> std::vector<VkVertexInputBindingDescription>&;
        static auto GetDefaultAttributeDescriptions() -> std::vector<VkVertexInputAttributeDescription>&;

        ~VulkanVertexBuffer() override;
    public:
        VulkanVertexBuffer(const VulkanVertexBuffer&) = delete;
        auto operator=(const VulkanVertexBuffer&) -> VulkanVertexBuffer& = delete;

        VulkanVertexBuffer(VulkanVertexBuffer&&) = delete;
        auto operator=(VulkanVertexBuffer&&) -> VulkanVertexBuffer& = delete;
    private:
        auto SetVertexData(const std::vector<float>& vertices) -> void;
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
        std::shared_ptr<VulkanDevice>   m_Device{};
        VkBuffer                        m_VertexBuffer{};
        VkDeviceMemory                  m_VertexBufferMemory{};
        ULongLong_T                     m_VertexCount{};
    };
}


#endif // KATE_ENGINE_VULKAN_VERTEX_BUFFER_HH
