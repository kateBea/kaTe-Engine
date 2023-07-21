//
// Created by kate on 7/3/23.
//

#ifndef KATE_ENGINE_VULKAN_RENDERER_HH
#define KATE_ENGINE_VULKAN_RENDERER_HH

#include <filesystem>

#include <volk.h>

#include <glm/glm.hpp>

#include <Tools/Common.hh>
#include <Renderer/RendererAPI.hh>

#include <Renderer/Buffers/VertexBuffer.hh>
#include <Renderer/Buffers/IndexBuffer.hh>
#include <Renderer/Model.hh>
#include <Renderer/Renderer.hh>
#include <Renderer/Material/Material.hh>
#include <Renderer/Vulkan/StandardMaterial.hh>
#include <Renderer/Vulkan/VulkanCommandPool.hh>
#include <Renderer/Vulkan/VulkanTexture2D.hh>
#include <Renderer/Vulkan/VulkanSwapChain.hh>

namespace kaTe {
    struct DrawData {
        std::shared_ptr<Model> ModelData{};
        std::shared_ptr<Material> Material{};

        glm::mat4 Model{};
        glm::mat4 View{};
        glm::mat4 Projection{};
    };

    class VulkanRenderer : public RendererAPI {
    public:
        explicit VulkanRenderer() = default;

        auto Init() -> void override;
        auto Shutdown() -> void override;

        auto EnableWireframeMode() -> void override;
        auto DisableWireframeMode() -> void override;

        auto SetClearColor(const glm::vec4& color) -> void override;
        auto SetClearColor(float red, float green, float blue, float alpha) -> void override;

        auto Draw(const DrawData& data) -> void;
        auto Draw(const RenderingData &data) -> void override;

        auto DrawIndexed(const std::shared_ptr<VertexBuffer> &vertexBuffer, const std::shared_ptr<IndexBuffer> &indexBuffer) -> void;

        auto SetViewPort(UInt32_T x, UInt32_T y, UInt32_T width, UInt32_T height) -> void override;
        auto OnEvent(Event& event) -> void override;

        auto GetCommandPool() -> VulkanCommandPool& { return *m_CommandPool; }

        ~VulkanRenderer() override = default;
    private:
        friend class StandardMaterial;
        friend class Application;

    private:
        /*************************************************************
        * HELPERS
        * ********************************************************+ */
        auto CreateCommandBuffers() -> void;
        auto DrawFrame(const Model &model) -> void;
        auto CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) -> void;
        auto RecordCommandBuffers(UInt32_T imageIndex, const Mesh& model) -> void;

    private:
        /*************************************************************
        * PRIVATE MEMBERS
        * ********************************************************+ */
        std::shared_ptr<StandardMaterial> m_DefaultMaterial{};
        std::shared_ptr<VulkanCommandPool> m_CommandPool{};
        std::vector<VkCommandBuffer> m_CommandBuffers{};

        glm::vec4 m_ClearColor{};

        VkViewport m_Viewport{};
        VkRect2D m_Scissor{};
    };
}


#endif //KATE_ENGINE_VULKAN_RENDERER_HH
