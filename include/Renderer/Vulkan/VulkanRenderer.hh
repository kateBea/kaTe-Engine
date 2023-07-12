//
// Created by kate on 6/29/23.
//

#ifndef KATE_ENGINE_VULKAN_RENDERER_HH
#define KATE_ENGINE_VULKAN_RENDERER_HH

#include <memory>
#include <vector>

#include <glm/glm.hpp>

#include <Tools/Common.hh>

#include <Platform/Window/MainWindow.hh>

#include <Renderer/RendererAPI.hh>

#include <Core/Events/Event.hh>
#include <Core/Events/AppEvents.hh>

#include <Renderer/Material/BaseShader.hh>
#include <Renderer/Buffers/IndexBuffer.hh>
#include <Renderer/Buffers/VertexBuffer.hh>


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

        auto OnEvent(Event& event) -> void override;

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

    public:
        // TODO: temporal
        friend class VulkanShader;
        friend class ImGuiLayer;
    private:
        // Probably need OnDestroy functions to control the order of Vulkan objects release
        // TODO: these should not be pointers
        std::shared_ptr<MainWindow>         m_Window{};

        auto OnWindowResize(WindowResizedEvent& event) -> bool;

    private:
        // VALIDATION LAYERS. Enabled on Debug builds
#if defined(NDEBUG)
    static constexpr bool m_EnableValidationLayers{ false };
#else
    static constexpr bool m_EnableValidationLayers{ true };
#endif
    private:
        struct SwapChainSupportDetails {
            VkSurfaceCapabilitiesKHR        Capabilities{};
            std::vector<VkSurfaceFormatKHR> Formats{};
            std::vector<VkPresentModeKHR>   PresentModes{};
        };

        struct QueueFamilyIndices {
            UInt32_T    GraphicsFamily{};
            UInt32_T    PresentFamily{};

            // TODO: use std::optional
            bool GraphicsFamilyHasValue{ false };
            bool PresentFamilyHasValue{ false };

            KT_NODISCARD auto IsComplete() const -> bool { return GraphicsFamilyHasValue && PresentFamilyHasValue; }
        };

        struct PipelineConfigInfo {
            VkPipelineViewportStateCreateInfo       ViewportInfo{};
            VkPipelineInputAssemblyStateCreateInfo  inputAssemblyInfo{};
            VkPipelineRasterizationStateCreateInfo  rasterizationInfo{};
            VkPipelineMultisampleStateCreateInfo    multisampleInfo{};
            VkPipelineColorBlendAttachmentState     colorBlendAttachment{};
            VkPipelineColorBlendStateCreateInfo     colorBlendInfo{};
            VkPipelineDepthStencilStateCreateInfo   depthStencilInfo{};
            VkPipelineLayout                        pipelineLayout{};
            std::vector<VkDynamicState>             DynamicStateEnables{};
            VkPipelineDynamicStateCreateInfo        DynamicStateInfo{};
            VkRenderPass                            renderPass{};
            UInt32_T                                subpass{};

            explicit PipelineConfigInfo() = default;

            PipelineConfigInfo(PipelineConfigInfo&& other) = default;
            auto operator=(PipelineConfigInfo&& other) -> PipelineConfigInfo& = default;

            PipelineConfigInfo(const PipelineConfigInfo&) = delete;
            auto operator=(const PipelineConfigInfo&) -> PipelineConfigInfo& = delete;
        };

        const std::vector<const char *> m_ValidationLayers{ "VK_LAYER_KHRONOS_validation" };

        const std::vector<const char *> m_DeviceRequiredExtensions{
                VK_KHR_SWAPCHAIN_EXTENSION_NAME,
                // VK_EXT_VERTEX_INPUT_DYNAMIC_STATE_EXTENSION_NAME,
                // VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME // not supported by the device this project was tested on but is required by VK_EXT_VERTEX_INPUT_DYNAMIC_STATE_EXTENSION_NAME
                // TODO: Implement static vertex buffer when the necessary extensions are not supported
        };

        // Submit at most 2 commands at once to the device command queues
        static constexpr Int32_T MAX_FRAMES_IN_FLIGHT{ 2 };

        KT_NODISCARD auto GetCommandPool() -> VkCommandPool { return m_CommandPool; }
        KT_NODISCARD auto GetSurface() -> VkSurfaceKHR { return m_Surface; }
        KT_NODISCARD auto GetGraphicsQueue() -> VkQueue { return m_GraphicsQueue; }
        KT_NODISCARD auto GetPresentQueue() -> VkQueue { return m_PresentQueue; }
        KT_NODISCARD auto GetSwapChainSupport() -> SwapChainSupportDetails { return QuerySwapChainSupport(m_PhysicalDevice); }
        KT_NODISCARD auto FindPhysicalQueueFamilies() -> QueueFamilyIndices { return FindQueueFamilies(m_PhysicalDevice); }
        KT_NODISCARD auto FindSupportedFormat(const std::vector<VkFormat> &candidates, VkImageTiling tiling, VkFormatFeatureFlags features) -> VkFormat;
        KT_NODISCARD auto BeginSingleTimeCommands() -> VkCommandBuffer;
        KT_NODISCARD auto FindMemoryType(UInt32_T typeFilter, VkMemoryPropertyFlags properties) -> UInt32_T;

        auto EndSingleTimeCommands(VkCommandBuffer commandBuffer) -> void;
        auto CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) -> void;
        auto CopyBufferToImage(VkBuffer buffer, VkImage image, UInt32_T width, UInt32_T height, UInt32_T layerCount) -> void;
        auto CreateImageWithInfo(const VkImageCreateInfo &imageInfo, VkMemoryPropertyFlags properties, VkImage &image, VkDeviceMemory& imageMemory) -> void;

        auto CreateInstance() -> void;
        auto SetupDebugMessenger() -> void;
        auto CreateSurface() -> void;
        auto PickPhysicalDevice() -> void;
        auto CreateLogicalDevice() -> void;
        auto CreateCommandPool() -> void;

        KT_NODISCARD auto IsDeviceSuitable(VkPhysicalDevice device) -> bool;
        KT_NODISCARD auto GetRequiredExtensions() const -> std::vector<const char*>;
        KT_NODISCARD auto FindQueueFamilies(VkPhysicalDevice device) -> QueueFamilyIndices;
        KT_NODISCARD auto CheckDeviceExtensionSupport(VkPhysicalDevice device) -> bool;
        KT_NODISCARD auto QuerySwapChainSupport(VkPhysicalDevice device) -> SwapChainSupportDetails;

        auto CheckValidationLayerSupport() -> bool;
        auto PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) -> void;
        auto HasGflwRequiredInstanceExtensions() -> void;


        KT_NODISCARD auto GetFrameBuffer(std::size_t index) -> VkFramebuffer { return m_SwapChainFrameBuffers[index]; }
        KT_NODISCARD auto GetRenderPass() -> VkRenderPass { return m_RenderPass; }
        KT_NODISCARD auto GetImageView(std::size_t index) -> VkImageView { return m_SwapChainImageViews[index]; }
        KT_NODISCARD auto GetImageCount() -> std::size_t { return m_SwapChainImages.size(); }
        KT_NODISCARD auto GetSwapChainImageFormat() -> VkFormat { return m_SwapChainImageFormat; }
        KT_NODISCARD auto GetSwapChainExtent() -> VkExtent2D { return m_SwapChainExtent; }

        KT_NODISCARD auto GetWidth() const -> UInt32_T { return m_SwapChainExtent.width; }
        KT_NODISCARD auto GetHeight() const -> UInt32_T { return m_SwapChainExtent.height; }
        KT_NODISCARD auto GetExtentAspectRatio() const -> float { return static_cast<float>(m_SwapChainExtent.width) / static_cast<float>(m_SwapChainExtent.height); }

        KT_NODISCARD auto FindDepthFormat() -> VkFormat;
        KT_NODISCARD auto AcquireNextImage(UInt32_T* imageIndex) -> VkResult;
        KT_NODISCARD auto SubmitCommandBuffers(const VkCommandBuffer *buffers, const UInt32_T *imageIndex) -> VkResult;

        auto CreateSwapChain() -> void;
        auto CreateImageViews() -> void;
        auto CreateDepthResources() -> void;
        auto CreateRenderPass() -> void;
        auto CreateFrameBuffers() -> void;
        auto CreateSyncObjects() -> void;

        auto InitSwapChain() -> void;
        auto DestroySwapChain() -> void;

        KT_NODISCARD static auto ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats) -> VkSurfaceFormatKHR;
        KT_NODISCARD static auto ChooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes) -> VkPresentModeKHR;
        KT_NODISCARD auto ChooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities) -> VkExtent2D;


        KT_NODISCARD static auto GetDefaultPipelineConfigInfo() -> PipelineConfigInfo;

        auto Bind(VkCommandBuffer commandBuffer) -> void;
        auto CreateShaderModule(const CharArray& srcCode, VkShaderModule* shaderModule) -> void;
        auto CreateGraphicsPipeline(const Path_T &vPath, const Path_T &fPath, const PipelineConfigInfo& config) -> void;


        VkInstance                      m_Instance{};
        VkDebugUtilsMessengerEXT        m_DebugMessenger{};
        VkPhysicalDevice                m_PhysicalDevice{};
        VkCommandPool                   m_CommandPool{};
        VkPhysicalDeviceProperties      m_Properties{};
        VkDevice                        m_Device{};
        VkSurfaceKHR                    m_Surface{};
        VkQueue                         m_GraphicsQueue{};
        VkQueue                         m_PresentQueue{};

        VkFormat                        m_SwapChainImageFormat{};
        VkExtent2D                      m_SwapChainExtent{};

        std::vector<VkFramebuffer>      m_SwapChainFrameBuffers{};
        VkRenderPass                    m_RenderPass{};

        std::vector<VkImage>            m_DepthImages{};
        std::vector<VkDeviceMemory>     m_DepthImageMemories{};
        std::vector<VkImageView>        m_DepthImageViews{};
        std::vector<VkImage>            m_SwapChainImages{};
        std::vector<VkImageView>        m_SwapChainImageViews{};

        VkExtent2D                      m_WindowExtent{};

        VkSwapchainKHR                  m_SwapChain{};

        std::vector<VkSemaphore>        m_ImageAvailableSemaphores{};
        std::vector<VkSemaphore>        m_RenderFinishedSemaphores{};
        std::vector<VkFence>            m_InFlightFences{};
        std::vector<VkFence>            m_ImagesInFlight{};
        std::size_t                     m_CurrentFrame{};

        VkPipeline                      m_GraphicsPipeline{};
        VkShaderModule                  m_VertShaderModule{};
        VkShaderModule                  m_FragShaderModule{};

        glm::vec4 m_ClearColor{};

        VkPipelineLayout                    m_PipelineLayout{};
        std::vector<VkCommandBuffer>        m_CommandBuffers{};
    };
}


#endif//KATE_ENGINE_VULKAN_RENDERER_HH
