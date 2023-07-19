/**
 * VulkanRenderer.cc
 * Created by kate on 7/3/23.
 * */
#include <memory>
#include <array>

#include <imgui.h>
#include <backends/imgui_impl_vulkan.h>
#include <backends/imgui_impl_glfw.h>

#include <glm/glm.hpp>

#include <volk.h>

#include <Core/Assert.hh>
#include <Core/Application.hh>

#include <Renderer/Material/Material.hh>
#include <Renderer/Vulkan/StandardMaterial.hh>
#include <Renderer/Vulkan/VulkanContext.hh>
#include <Renderer/Vulkan/VulkanRenderer.hh>
#include <Renderer/Vulkan/VulkanCommandPool.hh>
#include <Renderer/Vulkan/VulkanSwapChain.hh>
#include <Renderer/Vulkan/VulkanVertexBuffer.hh>
#include <Renderer/Vulkan/VulkanIndexBuffer.hh>

namespace kaTe {

    auto VulkanRenderer::Init() -> void {
        static VulkanRenderer singleInstance{};

        s_CommandPool = std::make_shared<VulkanCommandPool>();
        KT_ASSERT(s_CommandPool, "Command Pool pointer is NULL");
        s_CommandPool->OnCreate();

        RecreateSwapChain();
        CreateCommandBuffers();
        InitImGui();

        s_ClearColor = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
        s_DefaultMaterial = std::make_shared<StandardMaterial>();
    }

    auto VulkanRenderer::EnableWireframeMode() -> void {
        s_DefaultMaterial->EnableWireframe();
    }

    auto VulkanRenderer::DisableWireframeMode() -> void {
        s_DefaultMaterial->DisableWireframe();
    }

    auto VulkanRenderer::SetClearColor(const glm::vec4 &color) -> void {
        s_ClearColor = color;
    }

    auto VulkanRenderer::SetClearColor(float red, float green, float blue, float alpha) -> void {
        s_ClearColor.r = red;
        s_ClearColor.g = green;
        s_ClearColor.b = blue;
        s_ClearColor.a = alpha;
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

        s_SwapChain->OnRelease();
        s_CommandPool->OnRelease();

        vkDestroyDescriptorPool(VulkanContext::GetPrimaryLogicalDevice(), s_ImguiPool, nullptr);
        ImGui_ImplVulkan_Shutdown();
        s_DefaultMaterial->OnRelease();
    }

    auto VulkanRenderer::CreateCommandBuffers() -> void {
        s_CommandBuffers.resize(s_SwapChain->GetImageCount());

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = s_CommandPool->GetCommandPool();
        allocInfo.commandBufferCount = static_cast<UInt32_T>(s_CommandBuffers.size());

        if (vkAllocateCommandBuffers(VulkanContext::GetPrimaryLogicalDevice(), &allocInfo, s_CommandBuffers.data()) != VK_SUCCESS)
            throw std::runtime_error("Failed to allocate command buffers");
    }

    auto VulkanRenderer::DrawFrame(const Model& model) -> void {
        UInt32_T imageIndex{};
        ImGui::Render();

        auto result{ s_SwapChain->AcquireNextImage(&imageIndex) };
        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            RecreateSwapChain();
            return;
        }

        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
            throw std::runtime_error("failed to acquire swap chain image!");


        s_DefaultMaterial->UpdateUniformBuffers(s_SwapChain->GetCurrentFrame());

        for (const auto& mesh : model.GetMeshes())
            RecordCommandBuffers(imageIndex, mesh);

        result = s_SwapChain->SubmitCommandBuffers(&s_CommandBuffers[imageIndex], &imageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
            RecreateSwapChain();
            return;
        }

        if (result != VK_SUCCESS)
            throw std::runtime_error("failed to present swap chain image!");
    }

    auto VulkanRenderer::RecreateSwapChain() -> void {
        vkDeviceWaitIdle(VulkanContext::GetPrimaryLogicalDevice());
        auto appWindowExtent{ Application::Get().GetMainWindowPtr()->GetExtent() };
        VkExtent2D extent{ (UInt32_T)appWindowExtent.first, (UInt32_T)appWindowExtent.second };
        if (s_SwapChain)
            s_SwapChain->OnRelease();

        s_SwapChain = std::make_shared<VulkanSwapChain>(extent);
    }

    auto VulkanRenderer::RecordCommandBuffers(UInt32_T imageIndex, const Mesh& mesh) -> void {
        VkCommandBufferBeginInfo beginInfo{};

        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(s_CommandBuffers[imageIndex], &beginInfo) != VK_SUCCESS)
            throw std::runtime_error("Failed to begin recording command buffer");

        VkRenderPassBeginInfo renderPassInfo{};

        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = s_SwapChain->GetRenderPass();
        renderPassInfo.framebuffer = s_SwapChain->GetFrameBuffer(imageIndex);

        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.renderArea.extent = s_SwapChain->GetSwapChainExtent();

        std::array<VkClearValue, 2> clearValues{};

        clearValues[0].color = { {s_ClearColor.r, s_ClearColor.g, s_ClearColor.b, s_ClearColor.a } };
        clearValues[1].depthStencil = { 1.0f, 0 };

        renderPassInfo.clearValueCount = static_cast<UInt32_T>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(s_CommandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        VkViewport viewport{
            .x = 0.0f,
            .y = 0.0f,
            .width = static_cast<float>(s_SwapChain->GetSwapChainExtent().width),
            .height = static_cast<float>(s_SwapChain->GetSwapChainExtent().height),
            .minDepth = 0.0f,
            .maxDepth = 1.0f,
        };

        VkRect2D scissor{
            .offset{ 0, 0 },
            .extent{ s_SwapChain->GetSwapChainExtent() },
        };

        vkCmdSetViewport(s_CommandBuffers[imageIndex], 0, 1, &viewport);
        vkCmdSetScissor(s_CommandBuffers[imageIndex], 0, 1, &scissor);

        s_DefaultMaterial->GetPipeline().Bind(s_CommandBuffers[imageIndex]);
        s_DefaultMaterial->BindDescriptorSets(s_CommandBuffers[imageIndex]);

        std::dynamic_pointer_cast<VulkanIndexBuffer>(mesh.GetIndexBuffer())->Bind(s_CommandBuffers[imageIndex]);
        std::dynamic_pointer_cast<VulkanVertexBuffer>(mesh.GetVertexBuffer())->Bind(s_CommandBuffers[imageIndex]);
        vkCmdDrawIndexed(s_CommandBuffers[imageIndex], std::dynamic_pointer_cast<VulkanIndexBuffer>(mesh.GetIndexBuffer())->GetCount(), 1, 0, 0, 0);

        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), s_CommandBuffers[imageIndex]);

        vkCmdEndRenderPass(s_CommandBuffers[imageIndex]);

        if (vkEndCommandBuffer(s_CommandBuffers[imageIndex]) != VK_SUCCESS)
            throw std::runtime_error("Failed to record command buffer");
    }

    auto VulkanRenderer::InitImGui() -> void {
        std::array<VkDescriptorPoolSize, 11> poolSizes{};

        poolSizes[0] = {VK_DESCRIPTOR_TYPE_SAMPLER, 1000 };
        poolSizes[1] = {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 };
        poolSizes[2] = {VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 };
        poolSizes[3] = {VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 };
        poolSizes[4] = {VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 };
        poolSizes[5] = {VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 };
        poolSizes[6] = {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 };
        poolSizes[7] = {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 };
        poolSizes[8] = {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 };
        poolSizes[9] = {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 };
        poolSizes[10] = {VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 };

        VkDescriptorPoolCreateInfo poolCreateInfo{};
        poolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolCreateInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        poolCreateInfo.maxSets = 1000;
        poolCreateInfo.poolSizeCount = static_cast<UInt32_T>(poolSizes.size());
        poolCreateInfo.pPoolSizes = poolSizes.data();

        if (vkCreateDescriptorPool(VulkanContext::GetPrimaryLogicalDevice(), &poolCreateInfo, nullptr, &s_ImguiPool) != VK_SUCCESS)
            throw std::runtime_error("failed to create descriptor pool for ImGui!");

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

        // When viewports are enabled, we tweak WindowRounding/WindowBg so platform windows can
        // look identical to regular ones.
        ImGuiStyle& style{ ImGui::GetStyle() };
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        // Configure ImGui Style
        ImGui::StyleColorsDark();

        // Load fonts
        io.Fonts->AddFontFromFileTTF("../assets/Fonts/Open_Sans/static/OpenSans-Bold.ttf", 17.5);
        io.FontDefault = io.Fonts->AddFontFromFileTTF("../assets/Fonts/Open_Sans/OpenSans-VariableFont_wdth,wght.ttf", 17.5);

        GLFWwindow* window{ std::any_cast<GLFWwindow*>(Application::Get().GetMainWindowPtr()->GetNativeWindow()) };
        ImGui_ImplGlfw_InitForVulkan(window, true);

        ImGui_ImplVulkan_InitInfo initInfo{};
        initInfo.Instance = VulkanContext::GetInstance();
        initInfo.PhysicalDevice = VulkanContext::GetPrimaryPhysicalDevice();
        initInfo.Device = VulkanContext::GetPrimaryLogicalDevice();
        initInfo.Queue = VulkanContext::GetPrimaryLogicalDeviceQueuesData().GraphicsQueue;
        initInfo.DescriptorPool = s_ImguiPool;
        initInfo.MinImageCount = 3;
        initInfo.ImageCount = 3;
        initInfo.MSAASamples = VK_SAMPLE_COUNT_1_BIT;

        //CreateImGuiRenderPass();
        //CreateImGuiCommandPool();
        //CreateImGuiCommandBuffers();
        //CreateImGuiFrameBuffers();

        if (!ImGui_ImplVulkan_Init(&initInfo, s_SwapChain->GetRenderPass()))
            throw std::runtime_error("Failed to initialize Vulkan for ImGui");

        //execute a gpu command to upload imgui font textures
        auto command{ s_CommandPool->BeginSingleTimeCommands() };
        ImGui_ImplVulkan_CreateFontsTexture(command);
        s_CommandPool->EndSingleTimeCommands(command);

        //clear font textures from cpu data
        vkDeviceWaitIdle(VulkanContext::GetPrimaryLogicalDevice());
        ImGui_ImplVulkan_DestroyFontUploadObjects();
    }

    auto VulkanRenderer::Draw(const DrawData &data) -> void {
        s_DefaultMaterial->SetModelMatrix(data.Model);
        s_DefaultMaterial->SetProjectionMatrix(data.Projection);
        s_DefaultMaterial->SetViewMatrix(data.View);

        DrawFrame(*data.ModelData);
    }

    auto VulkanRenderer::CreateImGuiRenderPass() -> void {
        VkAttachmentDescription attachment{};
        attachment.format = s_SwapChain->GetSwapChainImageFormat();
        attachment.samples = VK_SAMPLE_COUNT_1_BIT;
        attachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
        attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        attachment.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference colorAttachment{};
        colorAttachment.attachment = 0;
        colorAttachment.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachment;

        VkSubpassDependency dependency{};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.srcAccessMask = 0; // or VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        VkRenderPassCreateInfo info{};
        info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        info.attachmentCount = 1;
        info.pAttachments = &attachment;
        info.subpassCount = 1;
        info.pSubpasses = &subpass;
        info.dependencyCount = 1;
        info.pDependencies = &dependency;

        if (vkCreateRenderPass(VulkanContext::GetPrimaryLogicalDevice(), &info, nullptr, &s_ImGuiRenderPass) != VK_SUCCESS)
            throw std::runtime_error("failed to create render pass!");
    }

    auto VulkanRenderer::CreateImGuiCommandPool() -> void {

    }

    auto VulkanRenderer::CreateImGuiCommandBuffers() -> void {

    }

    auto VulkanRenderer::CreateImGuiFrameBuffers() -> void {

    }
}