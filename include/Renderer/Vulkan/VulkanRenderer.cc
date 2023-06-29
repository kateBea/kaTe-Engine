//
// Created by kate on 6/29/23.
//

#include <memory>
#include <vector>

#include <glm/glm.hpp>

#include <Tools/Common.hh>

#include <Core/Application.hh>
#include <Renderer/RendererAPI.hh>

#include <Renderer/Material/BaseShader.hh>
#include <Renderer/Buffers/IndexBuffer.hh>
#include <Renderer/Buffers/VertexBuffer.hh>

#include <Renderer/Vulkan/VulkanVertexBuffer.hh>
#include <Renderer/Vulkan/VulkanPipeline.hh>
#include <Renderer/Vulkan/VulkanDevice.hh>
#include <Renderer/Vulkan/VulkanSwapChain.hh>

#include <Renderer/Vulkan/VulkanRenderer.hh>

namespace kaTe {

    auto VulkanRenderer::Init() -> void {
        std::shared_ptr<MainWindow> window{ std::dynamic_pointer_cast<MainWindow>(Application::GetPtr()->GetMainWindowPtr()) };
        m_Device = std::make_shared<VulkanDevice>(window);
        m_SwapChain = std::make_shared<VulkanSwapChain>(m_Device, window->GetExtent());

        CreatePipelineLayout();
        CreatePipeline();
        CreateCommandBuffers();
    }

    auto VulkanRenderer::Shutdown() -> void {
        vkDestroyPipelineLayout(m_Device->GetDevice(), m_PipelineLayout, nullptr);
    }

    auto VulkanRenderer::EnableWireframeMode() -> void {

    }

    auto VulkanRenderer::DisableWireframeMode() -> void {

    }

    auto VulkanRenderer::SetClearColor(const glm::vec4 &color) -> void {

    }

    auto VulkanRenderer::SetClearColor(float red, float green, float blue, float alpha) -> void {

    }

    auto VulkanRenderer::Clear(const RendererAPI::BufferBits &bufferBits) -> void {

    }

    auto VulkanRenderer::SetViewPort(UInt32_T x, UInt32_T y, UInt32_T width, UInt32_T height) -> void {

    }

    auto VulkanRenderer::DrawIndexed(const std::shared_ptr<VertexBuffer> &vertexBuffer, const std::shared_ptr<IndexBuffer> &indexBuffer) -> void {

    }

    auto VulkanRenderer::DrawIndexed(const std::shared_ptr<BaseShader> &shader, const std::shared_ptr<VertexBuffer> &vertexBuffer, const std::shared_ptr<IndexBuffer> &indexBuffer) -> void {

    }

    auto VulkanRenderer::Draw(const std::shared_ptr<VertexBuffer> &vertexBuffer) -> void {

    }

    auto VulkanRenderer::Draw(const std::shared_ptr<VertexBuffer> &vertexBuffer, const std::shared_ptr<IndexBuffer> &indexBuffer) -> void {

    }

    auto VulkanRenderer::Draw(const std::shared_ptr<BaseShader> &shader, const std::shared_ptr<VertexBuffer> &vertexBuffer) -> void {

    }

    auto VulkanRenderer::Draw(const std::shared_ptr<BaseShader> &shader, const std::shared_ptr<VertexBuffer> &vertexBuffer, const std::shared_ptr<IndexBuffer> &indexBuffer) -> void {

    }

    auto VulkanRenderer::SetDefaultShader(const Path_T& vertShaderPath, const Path_T& pixelShaderPath) -> void {

    }

    auto VulkanRenderer::CreatePipelineLayout() -> void {
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};

        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.pPushConstantRanges = nullptr;

        if (vkCreatePipelineLayout(m_Device->GetDevice(), &pipelineLayoutInfo, nullptr, &m_PipelineLayout) != VK_SUCCESS)
            throw std::runtime_error("Failed to create pipeline layout");
    }

    auto VulkanRenderer::CreatePipeline() -> void {
        std::string vertSpirVPath{ "../assets/basicVert.sprv" };
        std::string pixelSpirVPath{ "../assets/basicFrag.sprv" };

        // IMPORTANT: use m_SwapChain width and height as they do not necessarily match the window's
        auto pipelineConfig{ VulkanPipeline::DefaultPipelineConfigInfo(m_SwapChain->GetWidth(), m_SwapChain->GetHeight()) };

        pipelineConfig.renderPass = m_SwapChain->GetRenderPass();
        pipelineConfig.pipelineLayout = m_PipelineLayout;
        m_Pipeline = std::make_shared<VulkanPipeline>(m_Device, vertSpirVPath, pixelSpirVPath, pipelineConfig);
    }

    auto VulkanRenderer::CreateCommandBuffers() -> void {
        m_CommandBuffers.resize(m_SwapChain->GetImageCount());

        VkCommandBufferAllocateInfo allocInfo{};

        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = m_Device->GetCommandPool();
        allocInfo.commandBufferCount = static_cast<UInt32_T>(m_CommandBuffers.size());

        if (vkAllocateCommandBuffers(m_Device->GetDevice(), &allocInfo, m_CommandBuffers.data()) != VK_SUCCESS)
            throw std::runtime_error("Failed to allocate command buffers");

        for (std::size_t i{}; i < m_CommandBuffers.size(); ++i) {
            VkCommandBufferBeginInfo beginInfo{};

            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

            if (vkBeginCommandBuffer(m_CommandBuffers[i], &beginInfo) != VK_SUCCESS)
                throw std::runtime_error("Failed to begin recording command buffer");

            VkRenderPassBeginInfo renderPassInfo{};

            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = m_SwapChain->GetRenderPass();
            renderPassInfo.framebuffer = m_SwapChain->GetFrameBuffer(i);

            renderPassInfo.renderArea.offset = { 0, 0 };
            renderPassInfo.renderArea.extent = m_SwapChain->GetSwapChainExtent();

            std::array<VkClearValue, 2> clearValues{};

            clearValues[0].color = { {0.1f, 0.1f, 0.1f, 1.0f} };
            clearValues[1].depthStencil = { 1.0f, 0 };

            renderPassInfo.clearValueCount = static_cast<UInt32_T>(clearValues.size());
            renderPassInfo.pClearValues = clearValues.data();

            vkCmdBeginRenderPass(m_CommandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
            m_Pipeline->Bind(m_CommandBuffers[i]);

            m_VertexBuffer->Bind(m_CommandBuffers[i]);
            m_VertexBuffer->Draw(m_CommandBuffers[i]);

            vkCmdEndRenderPass((m_CommandBuffers[i]));

            if (vkEndCommandBuffer(m_CommandBuffers[i]) != VK_SUCCESS)
                throw std::runtime_error("Failed to record command buffer");
        }
    }
}
