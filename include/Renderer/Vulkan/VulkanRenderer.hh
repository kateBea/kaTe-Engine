//
// Created by kate on 7/3/23.
//

#ifndef KATE_ENGINE_VULKAN_RENDERER_HH
#define KATE_ENGINE_VULKAN_RENDERER_HH

#include <filesystem>

#include <volk.h>

#include <glm/glm.hpp>

#include <Tools/Common.hh>
#include <Renderer/Buffers/VertexBuffer.hh>
#include <Renderer/Buffers/IndexBuffer.hh>
#include <Renderer/Model.hh>

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

    class VulkanRenderer {
    public:
        explicit VulkanRenderer() = default;

        static auto Init() -> void;
        static auto Shutdown() -> void;

        static auto EnableWireframeMode() -> void;
        static auto DisableWireframeMode() -> void;

        static auto SetClearColor(const glm::vec4& color) -> void;
        static auto SetClearColor(float red, float green, float blue, float alpha) -> void;

        static auto SetViewPort(UInt32_T x, UInt32_T y, UInt32_T width, UInt32_T height) -> void;

        static auto Draw(const DrawData& data) -> void;

        static auto GetCommandPool() -> VulkanCommandPool& { return *s_CommandPool; }

        ~VulkanRenderer() = default;
    private:
        friend class StandardMaterial;
        friend class Application;

    private:
        static auto CreateCommandBuffers() -> void;
        static auto DrawFrame(const Model &model) -> void;

        static auto CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) -> void;

        // ImGui
        static auto InitImGui() -> void;
       static auto CreateImGuiRenderPass() -> void;
       static auto CreateImGuiCommandPool() -> void;
       static auto CreateImGuiCommandBuffers() -> void;
       static auto CreateImGuiFrameBuffers() -> void;

        static auto RecreateSwapChain() -> void;
        static auto RecordCommandBuffers(UInt32_T imageIndex, const Mesh& model) -> void;
    private:
        /*************************************************************
        * FOR IMGUI
        * ********************************************************+ */
        inline static VkDescriptorPool s_ImGuiDescriptorPool{};
        inline static VkRenderPass s_ImGuiRenderPass{};
        inline static std::vector<VkFramebuffer> s_ImGuiFrameBuffers{};
        inline static VkCommandPool s_ImGuiCommandPool{};
        inline static std::vector<VkCommandBuffer> s_ImGuiCommandBuffers{};


        /*************************************************************
        * PRIVATE STATIC MEMBER VARIABLES
        * ********************************************************+ */
        inline static std::shared_ptr<StandardMaterial> s_DefaultMaterial{};
        inline static std::shared_ptr<VulkanSwapChain>  s_SwapChain{};
        inline static std::shared_ptr<VulkanCommandPool> s_CommandPool{};

        inline static VkDescriptorPool s_ImguiPool{};
        inline static std::vector<VkCommandBuffer> s_CommandBuffers{};

        inline static glm::vec4 s_ClearColor{};
    };
}


#endif //KATE_ENGINE_VULKAN_RENDERER_HH
