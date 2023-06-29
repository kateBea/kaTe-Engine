//
// Created by kate on 6/29/23.
//

// C++ Standard Library
#include <any>

// Third-Party Libraries
#include <volk.h>
#include <GLFW/glfw3.h>

// Project Headers
#include <Tools/Common.hh>

#include <Core/Assert.hh>
#include <Core/Logger.hh>

#include <Renderer/RenderCommand.hh>
#include <Renderer/Vulkan/VulkanSwapChain.hh>
#include <Renderer/Vulkan/VulkanContext.hh>

namespace kaTe {

    auto VulkanContext::Init(std::any windowHandle) -> void {
        VkResult ret{ volkInitialize() };
        KT_ASSERT(ret == VK_SUCCESS, "Failed to init VOLK");

        // Because GLFW was originally designed to create an OpenGL context,
        // we need to tell it to not create an OpenGL context with a subsequent call
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        try {
            // We expect the native window for Linux Window to be a GLFWwindow*
            m_Handle = std::any_cast<GLFWwindow*>(windowHandle);
            KT_ASSERT(m_Handle, "Window handle for OpenGL context initialization is NULL");

            // Because GLFW was originally designed to create an OpenGL context,
            // we need to tell it to not create an OpenGL context with a subsequent call
            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

            KT_ASSERT(m_VOLKInitSuccess, "Failed to initialize VOLK");

        }
        catch (const std::bad_any_cast& exception) {
            KATE_APP_LOGGER_ERROR("Exception thrown std::any_cast at OpenGLContext::Init(). What: {}", exception.what());
        }
    }

    auto VulkanContext::ShutDown() -> void {

    }

    auto VulkanContext::DrawFrame() -> void {
        UInt32_T imageIndex{};
        try {
            std::shared_ptr<VulkanSwapChain> swampChain{ std::any_cast<std::shared_ptr<VulkanSwapChain>>(RenderCommand::GetSwapChain()) };
            std::vector<VkCommandBuffer> commandBuffers{ std::any_cast<std::vector<VkCommandBuffer>>(RenderCommand::GetCommandBuffers()) };

            VkResult result{ swampChain->AcquireNextImage(&imageIndex) };

            if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
                // to be properly handled in the future since we enter this block too when the window is resized
                throw std::runtime_error("Failed to acquire swap chain image");

            result = swampChain->SubmitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
            if (result != VK_SUCCESS)
                // to be properly handled in the future since we enter this block too when the window is resized
                throw std::runtime_error("Failed to present swap chain image");
        }
        catch (std::bad_any_cast& except) {
            KATE_APP_LOGGER_CRITICAL("std::any_cast exception. What: {}", except.what());
        }
    }
}
