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

#include "Renderer/Vulkan/VulkanRenderer.hh"
#include <Renderer/RenderCommand.hh>
#include <Renderer/Vulkan/VulkanContext.hh>
#include <Renderer/Vulkan/VulkanSwapChain.hh>

namespace kaTe {

    auto VulkanContext::Init(std::any windowHandle) -> void {
        VkResult ret{ volkInitialize() };
        m_VOLKInitSuccess = ret == VK_SUCCESS;
        KT_ASSERT(m_VOLKInitSuccess, "Failed to init VOLK");


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

    }
}
