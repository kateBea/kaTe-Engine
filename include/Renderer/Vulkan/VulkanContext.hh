//
// Created by kate on 6/29/23.
//

#ifndef KATE_ENGINE_VULKAN_CONTEXT_HH
#define KATE_ENGINE_VULKAN_CONTEXT_HH

#include <any>

#include <GLFW/glfw3.h>

#include <Platform/Window/MainWindow.hh>
#include <Renderer/RenderContext.hh>

namespace kaTe {
    class VulkanContext : public RenderContext {
        explicit VulkanContext() = default;

        auto Init(std::any windowHandle) -> void override;
        auto ShutDown() -> void override;
        auto DrawFrame() -> void override;

        ~VulkanContext() override = default;

    public:
        // Forbidden operations on Contexts
        VulkanContext(const VulkanContext&) = delete;
        auto operator=(const VulkanContext&) -> VulkanContext& = delete;

        VulkanContext(VulkanContext&&) = delete;
        auto operator=(VulkanContext&&) -> VulkanContext& = delete;
    private:
        GLFWwindow* m_Handle{};
        bool m_VOLKInitSuccess{ false };
    };
}


#endif//KATE_ENGINE_VULKAN_CONTEXT_HH
