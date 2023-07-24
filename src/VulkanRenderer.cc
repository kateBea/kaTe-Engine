/**
 * VulkanRenderer.cc
 * Created by kate on 7/3/23.
 * */
#include <memory>
#include <array>

#include <volk.h>

#include <imgui.h>
#include <backends/imgui_impl_vulkan.h>
#include <backends/imgui_impl_glfw.h>

#include <Core/Assert.hh>
#include <Core/Application.hh>

#include <Renderer/Vulkan/VulkanCommandPool.hh>
#include <Renderer/Vulkan/VulkanContext.hh>
#include <Renderer/Vulkan/VulkanIndexBuffer.hh>
#include <Renderer/Vulkan/VulkanRenderer.hh>
#include <Renderer/Vulkan/VulkanStandardMaterial.hh>
#include <Renderer/Vulkan/VulkanVertexBuffer.hh>

namespace kaTe {

    auto VulkanRenderer::Init() -> void {
        m_CommandPool = std::make_shared<VulkanCommandPool>();
        KT_ASSERT(m_CommandPool, "Command Pool pointer is NULL");
        m_CommandPool->OnCreate(VkCommandPoolCreateInfo());

        VulkanContext::RecreateSwapChain();
        CreateCommandBuffers();

        m_ClearColor = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
        m_DefaultMaterial = std::make_shared<VulkanStandardMaterial>();
    }

    auto VulkanRenderer::EnableWireframeMode() -> void {
        m_DefaultMaterial->EnableWireframe();
    }

    auto VulkanRenderer::DisableWireframeMode() -> void {
        m_DefaultMaterial->DisableWireframe();
    }

    auto VulkanRenderer::SetClearColor(const glm::vec4 &color) -> void {
        m_ClearColor = color;
    }

    auto VulkanRenderer::SetClearColor(float red, float green, float blue, float alpha) -> void {
        m_ClearColor.r = red;
        m_ClearColor.g = green;
        m_ClearColor.b = blue;
        m_ClearColor.a = alpha;
    }

    auto VulkanRenderer::SetViewPort(UInt32_T x, UInt32_T y, UInt32_T width, UInt32_T height) -> void {

    }

    auto VulkanRenderer::CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) -> void {
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = usage;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(VulkanContext::GetPrimaryLogicalDevice(), &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
            throw std::runtime_error("failed to create buffer!");

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
            throw std::runtime_error("failed to allocate buffer memory!");

        vkBindBufferMemory(VulkanContext::GetPrimaryLogicalDevice(), buffer, bufferMemory, 0);
    }

    auto VulkanRenderer::Shutdown() -> void {
        vkDeviceWaitIdle(VulkanContext::GetPrimaryLogicalDevice());

        m_CommandPool->OnRelease();

        ImGui_ImplVulkan_Shutdown();
        m_DefaultMaterial->OnRelease();
    }

    auto VulkanRenderer::CreateCommandBuffers() -> void {
        m_CommandBuffers.resize(VulkanContext::GetSwapChain()->GetImageCount());

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = m_CommandPool->GetCommandPool();
        allocInfo.commandBufferCount = static_cast<UInt32_T>(m_CommandBuffers.size());

        if (vkAllocateCommandBuffers(VulkanContext::GetPrimaryLogicalDevice(), &allocInfo, m_CommandBuffers.data()) != VK_SUCCESS)
            throw std::runtime_error("Failed to allocate command buffers");
    }

    auto VulkanRenderer::DrawFrame(const Model& model) -> void {
        UInt32_T imageIndex{};
        ImGui::Render();

        auto result{ VulkanContext::GetSwapChain()->AcquireNextImage(&imageIndex) };
        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            VulkanContext::RecreateSwapChain();
            return;
        }

        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
            throw std::runtime_error("failed to acquire swap chain image!");


        m_DefaultMaterial->UpdateUniformBuffers(VulkanContext::GetSwapChain()->GetCurrentFrame());

        for (const auto& mesh : model.GetMeshes())
            RecordCommandBuffers(imageIndex, mesh);

        result = VulkanContext::GetSwapChain()->SubmitCommandBuffers(&m_CommandBuffers[imageIndex], imageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
            VulkanContext::RecreateSwapChain();
            return;
        }

        if (result != VK_SUCCESS)
            throw std::runtime_error("failed to present swap chain image!");
    }

    auto VulkanRenderer::RecordCommandBuffers(UInt32_T imageIndex, const Mesh& mesh) -> void {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(m_CommandBuffers[imageIndex], &beginInfo) != VK_SUCCESS)
            throw std::runtime_error("Failed to begin recording command buffer");

        VkRenderPassBeginInfo renderPassInfo{};

        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = VulkanContext::GetSwapChain()->GetRenderPass();
        renderPassInfo.framebuffer = VulkanContext::GetSwapChain()->GetFrameBuffer(imageIndex);

        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.renderArea.extent = VulkanContext::GetSwapChain()->GetSwapChainExtent();

        std::array<VkClearValue, 2> clearValues{};

        clearValues[0].color = { {m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a } };
        clearValues[1].depthStencil = { 1.0f, 0 };

        renderPassInfo.clearValueCount = static_cast<UInt32_T>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(m_CommandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        VkViewport viewport{
            .x = 0.0f,
            .y = 0.0f,
            .width = static_cast<float>(VulkanContext::GetSwapChain()->GetSwapChainExtent().width),
            .height = static_cast<float>(VulkanContext::GetSwapChain()->GetSwapChainExtent().height),
            .minDepth = 0.0f,
            .maxDepth = 1.0f,
        };

        VkRect2D scissor{
            .offset{ 0, 0 },
            .extent{ VulkanContext::GetSwapChain()->GetSwapChainExtent() },
        };

        vkCmdSetViewport(m_CommandBuffers[imageIndex], 0, 1, &viewport);
        vkCmdSetScissor(m_CommandBuffers[imageIndex], 0, 1, &scissor);

        m_DefaultMaterial->GetPipeline().Bind(m_CommandBuffers[imageIndex]);
        m_DefaultMaterial->BindDescriptorSets(m_CommandBuffers[imageIndex]);

        std::dynamic_pointer_cast<VulkanIndexBuffer>(mesh.GetIndexBuffer())->Bind(m_CommandBuffers[imageIndex]);
        std::dynamic_pointer_cast<VulkanVertexBuffer>(mesh.GetVertexBuffer())->Bind(m_CommandBuffers[imageIndex]);
        vkCmdDrawIndexed(m_CommandBuffers[imageIndex], std::dynamic_pointer_cast<VulkanIndexBuffer>(mesh.GetIndexBuffer())->GetCount(), 1, 0, 0, 0);
        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), m_CommandBuffers[imageIndex]);

        vkCmdEndRenderPass(m_CommandBuffers[imageIndex]);

        if (vkEndCommandBuffer(m_CommandBuffers[imageIndex]) != VK_SUCCESS)
            throw std::runtime_error("Failed to record command buffer");
    }


    auto VulkanRenderer::Draw(const DrawData &data) -> void {
        m_DefaultMaterial->SetModelMatrix(data.Model);
        m_DefaultMaterial->SetProjectionMatrix(data.Projection);
        m_DefaultMaterial->SetViewMatrix(data.View);

        DrawFrame(*data.ModelData);
    }

    auto VulkanRenderer::DrawIndexed(const std::shared_ptr<VertexBuffer> &vertexBuffer, const std::shared_ptr<IndexBuffer> &indexBuffer) -> void {

    }

    auto VulkanRenderer::OnEvent(Event &event) -> void {

    }

    auto VulkanRenderer::Draw(const RenderingData &data) -> void {

    }
}