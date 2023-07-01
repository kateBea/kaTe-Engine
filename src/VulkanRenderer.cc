//
// Created by kate on 6/29/23.
//

#include <memory>
#include <vector>

#include "glm/glm.hpp"

#include "Tools/Common.hh"

#include "Core/Application.hh"
#include "Renderer/RendererAPI.hh"

#include "Renderer/Buffers/IndexBuffer.hh"
#include "Renderer/Buffers/VertexBuffer.hh"
#include "Renderer/Material/BaseShader.hh"

#include "Renderer/Vulkan/VulkanDevice.hh"
#include "Renderer/Vulkan/VulkanPipeline.hh"
#include "Renderer/Vulkan/VulkanSwapChain.hh"
#include "Renderer/Vulkan/VulkanVertexBuffer.hh"

#include "Renderer/Vulkan/VulkanRenderer.hh"

namespace kaTe {

    auto VulkanRenderer::Init() -> void {
        // For now it expects a MainWindow which is a wrapper around a GLFW window
        m_Window = std::dynamic_pointer_cast<MainWindow>(Application::GetPtr()->GetMainWindowPtr());
        m_Device = std::make_shared<VulkanDevice>(m_Window);
        m_SwapChain = std::make_shared<VulkanSwapChain>(m_Device, m_Window->GetExtent());

        CreatePipelineLayout();
        CreatePipeline();
        CreateCommandBuffers();
    }

    auto VulkanRenderer::Shutdown() -> void {
        // Wait until all operations have completed
        vkDeviceWaitIdle(m_Device->GetDevice());

        //m_SwapChain->OnDestroy();
        //vkDestroyPipelineLayout(m_Device->GetDevice(), m_PipelineLayout, nullptr);

        m_SwapChain->OnDestroy();
        m_Pipeline->OnDestroy();
        m_Device->OnDestroy();
    }

    auto VulkanRenderer::EnableWireframeMode() -> void {

    }

    auto VulkanRenderer::DisableWireframeMode() -> void {

    }

    auto VulkanRenderer::SetClearColor(const glm::vec4& color) -> void {
        m_ClearColor = color;
    }

    auto VulkanRenderer::SetClearColor(float red, float green, float blue, float alpha) -> void {
        m_ClearColor = glm::vec4(red, green, blue, alpha);
    }

    auto VulkanRenderer::Clear(const RendererAPI::BufferBits &bufferBits) -> void {

    }

    auto VulkanRenderer::SetViewPort(UInt32_T x, UInt32_T y, UInt32_T width, UInt32_T height) -> void {

    }

    auto VulkanRenderer::DrawIndexed(const std::shared_ptr<VertexBuffer> &vertexBuffer, const std::shared_ptr<IndexBuffer> &indexBuffer) -> void {

    }

    auto VulkanRenderer::DrawIndexed(const std::shared_ptr<BaseShader> &shader, const std::shared_ptr<VertexBuffer> &vertexBuffer, const std::shared_ptr<IndexBuffer> &indexBuffer) -> void {

    }

    auto VulkanRenderer::Draw(const std::shared_ptr<VertexBuffer>& vertexBuffer) -> void {
        UInt32_T imageIndex{};

        auto result{ m_SwapChain->AcquireNextImage(&imageIndex) };
        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            RecreateSwapChain();
            return;
        }
        else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("failed to acquire swap chain image!");
        }

        RecordCommandBuffers(imageIndex, vertexBuffer);
        result = m_SwapChain->SubmitCommandBuffers(&m_CommandBuffers[imageIndex], &imageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
            RecreateSwapChain();
            return;
        }
        else if (result != VK_SUCCESS) {
            throw std::runtime_error("failed to present swap chain image!");
        }
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
        auto pipelineConfig{ VulkanPipeline::GetDefaultPipelineConfigInfo() };

        pipelineConfig.renderPass = m_SwapChain->GetRenderPass();
        pipelineConfig.pipelineLayout = m_PipelineLayout;
        m_Pipeline = std::make_shared<VulkanPipeline>(m_Device, "../assets/shaders/basicVert.sprv", "../assets/shaders/basicFrag.sprv", pipelineConfig);
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
    }

    auto VulkanRenderer::RecreateSwapChain() -> void {
        vkDeviceWaitIdle(m_Device->GetDevice());
        // Wait for the current swap chain to stop being used
        auto extent{ m_Window->GetExtent() };

        // do not process event's while this window has no size, e.g. when minimized
        while (extent.width == 0 || extent.height == 0) {
            extent = m_Window->GetExtent();
            glfwWaitEvents();
        }

        if (m_SwapChain)
            m_SwapChain->OnDestroy();

        m_SwapChain = std::make_shared<VulkanSwapChain>(m_Device, extent);

        static bool first{ true };
        // No need if render passes from swap chain and pipeline are compatible
        if (first) {
            first = false;
            CreatePipeline();
        }
    }

    auto VulkanRenderer::RecordCommandBuffers(UInt32_T imageIndex, const std::shared_ptr<VertexBuffer>& vertexBuffer) -> void {
        VkCommandBufferBeginInfo beginInfo{};

        auto vertices{ std::dynamic_pointer_cast<VulkanVertexBuffer>(vertexBuffer) };

        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(m_CommandBuffers[imageIndex], &beginInfo) != VK_SUCCESS)
            throw std::runtime_error("Failed to begin recording command buffer");

        VkRenderPassBeginInfo renderPassInfo{};

        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = m_SwapChain->GetRenderPass();
        renderPassInfo.framebuffer = m_SwapChain->GetFrameBuffer(imageIndex);

        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.renderArea.extent = m_SwapChain->GetSwapChainExtent();

        std::array<VkClearValue, 2> clearValues{};

        clearValues[0].color = { {m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a} };
        clearValues[1].depthStencil = { 1.0f, 0 };

        renderPassInfo.clearValueCount = static_cast<UInt32_T>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(m_CommandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(m_SwapChain->GetSwapChainExtent().width);
        viewport.height = static_cast<float>(m_SwapChain->GetSwapChainExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        VkRect2D scissor{ .offset{ 0, 0 }, .extent{ m_SwapChain->GetSwapChainExtent() } };
        vkCmdSetViewport(m_CommandBuffers[imageIndex], 0, 1, &viewport);
        vkCmdSetScissor(m_CommandBuffers[imageIndex], 0, 1, &scissor);

        m_Pipeline->Bind(m_CommandBuffers[imageIndex]);
        vertices->Bind(m_CommandBuffers[imageIndex]);

        // draw call
        vkCmdDraw(m_CommandBuffers[imageIndex], vertices->GetVertexCount() , 1, 0, 0);

        vkCmdEndRenderPass((m_CommandBuffers[imageIndex]));

        if (vkEndCommandBuffer(m_CommandBuffers[imageIndex]) != VK_SUCCESS)
            throw std::runtime_error("Failed to record command buffer");
    }
}
