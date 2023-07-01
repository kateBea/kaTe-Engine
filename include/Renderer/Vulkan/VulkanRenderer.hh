//
// Created by kate on 6/29/23.
//

#ifndef KATE_ENGINE_VULKAN_RENDERER_HH
#define KATE_ENGINE_VULKAN_RENDERER_HH

#include <memory>
#include <vector>

#include <glm/glm.hpp>

#include <Tools/Common.hh>

#include <Renderer/RendererAPI.hh>

#include <Renderer/Material/BaseShader.hh>
#include <Renderer/Buffers/IndexBuffer.hh>
#include <Renderer/Buffers/VertexBuffer.hh>

#include <Renderer/Vulkan/VulkanPipeline.hh>
#include <Renderer/Vulkan/VulkanDevice.hh>
#include <Renderer/Vulkan/VulkanSwapChain.hh>
#include <Renderer/Vulkan/VulkanVertexBuffer.hh>


namespace kaTe {
    class VulkanRenderer : public RendererAPI {
    public:
        explicit VulkanRenderer() = default;

        auto Init() -> void override;
        auto Shutdown() -> void override;

        auto EnableWireframeMode() -> void override;
        auto DisableWireframeMode() -> void override;

        auto SetClearColor(const glm::vec4& color) -> void override;
        auto SetClearColor(float red, float green, float blue, float alpha) -> void override;
        auto Clear(const BufferBits& bufferBits) -> void override;
        auto SetViewPort(UInt32_T x, UInt32_T y, UInt32_T width, UInt32_T height) -> void override;

        auto DrawIndexed(const std::shared_ptr<VertexBuffer> &vertexBuffer, const std::shared_ptr<IndexBuffer> &indexBuffer) -> void override;
        auto DrawIndexed(const std::shared_ptr<BaseShader> &shader, const std::shared_ptr<VertexBuffer> &vertexBuffer, const std::shared_ptr<IndexBuffer> &indexBuffer) -> void override;

        // virtual auto draw(const Mesh& mesh) -> void = 0;
        auto Draw(const std::shared_ptr<VertexBuffer> &vertexBuffer) -> void override;
        auto Draw(const std::shared_ptr<VertexBuffer> &vertexBuffer, const std::shared_ptr<IndexBuffer> &indexBuffer) -> void override;
        // There's no reason to draw a mesh indexed as it may probably have its own indices

        // virtual auto draw(const Shader& shader, const Mesh& mesh) -> void = 0;
        auto Draw(const std::shared_ptr<BaseShader> &shader, const std::shared_ptr<VertexBuffer> &vertexBuffer) -> void override;
        auto Draw(const std::shared_ptr<BaseShader> &shader, const std::shared_ptr<VertexBuffer> &vertexBuffer, const std::shared_ptr<IndexBuffer> &indexBuffer) -> void override;

        auto SetDefaultShader(const Path_T& vertShaderPath, const Path_T& pixelShaderPath) -> void;

        KT_NODISCARD auto GetSwapChain() -> std::any override { return m_SwapChain; }
        KT_NODISCARD auto GetCommandBuffers() -> std::any override { return m_CommandBuffers; }

        ~VulkanRenderer() override = default;
    public:
        // Forbidden operations
        VulkanRenderer(const VulkanRenderer&) = delete;
        auto operator=(const VulkanRenderer&) -> VulkanRenderer& = delete;

        VulkanRenderer(VulkanRenderer&&) = delete;
        auto operator=(VulkanRenderer&&) -> VulkanRenderer& = delete;
    private:
        friend class VulkanContext;
        friend class VulkanVertexBuffer;
    private:
        auto CreatePipelineLayout() -> void;
        auto CreatePipeline() -> void;
        auto CreateCommandBuffers() -> void;

        auto RecreateSwapChain() -> void;
        auto RecordCommandBuffers(UInt32_T imageIndex, const std::shared_ptr<VertexBuffer> &vertexBuffer) -> void;

    private:
        friend class VulkanShader;
    private:
        // Probably need OnDestroy functions to control the order of Vulkan objects release
        // TODO: these should not be pointers
        std::shared_ptr<MainWindow>         m_Window{};
        std::shared_ptr<VulkanDevice>       m_Device{};
        std::shared_ptr<VulkanSwapChain>    m_SwapChain{};
        std::shared_ptr<VulkanPipeline>     m_Pipeline{};

        glm::vec4 m_ClearColor{};

        VkPipelineLayout                    m_PipelineLayout{};
        std::vector<VkCommandBuffer>        m_CommandBuffers{};
    };
}


#endif//KATE_ENGINE_VULKAN_RENDERER_HH
