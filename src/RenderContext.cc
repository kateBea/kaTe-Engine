//
// Created by kate on 7/19/2023.
//

#include <Renderer/Renderer.hh>
#include <Renderer/RenderContext.hh>

#include <Renderer/Vulkan/VulkanContext.hh>
#include <Renderer/OpenGL/OpenGLContext.hh>

namespace kaTe {
    auto RenderContext::Init(std::shared_ptr<Window> windowHandle) -> void {
        s_WindowHandle = std::move(windowHandle);

        switch (s_ActiveAPI) {
            case Renderer::GraphicsAPI::OPENGL_API:
                OpenGLContext::Init(s_WindowHandle);
                break;
            case Renderer::GraphicsAPI::VULKAN_API:
                VulkanContext::Init(s_WindowHandle);
                break;
        }
    }

    auto RenderContext::ShutDown() -> void {
        switch (s_ActiveAPI) {
            case Renderer::GraphicsAPI::OPENGL_API:
                OpenGLContext::ShutDown();
                break;
            case Renderer::GraphicsAPI::VULKAN_API:
                VulkanContext::ShutDown();
                break;
        }
    }

    auto RenderContext::Draw() -> void {
        switch (s_ActiveAPI) {
            case Renderer::GraphicsAPI::OPENGL_API:
                OpenGLContext::Draw();
                break;
        }
    }
    auto RenderContext::IsVSyncActive() -> bool {
        switch (s_ActiveAPI) {
            case Renderer::GraphicsAPI::OPENGL_API:
                return OpenGLContext::IsVSyncActive();
                break;
            case Renderer::GraphicsAPI::VULKAN_API:
                // TODO:
                break;
            default: return false;
        }
    }

    auto RenderContext::EnableVSync() -> void {
        switch (s_ActiveAPI) {
            case Renderer::GraphicsAPI::OPENGL_API:
                OpenGLContext::EnableVSync();
                break;
            case Renderer::GraphicsAPI::VULKAN_API:
                // TODO:
                break;
        }
    }

    auto RenderContext::DisableVSync() -> void {
        switch (s_ActiveAPI) {
            case Renderer::GraphicsAPI::OPENGL_API:
                OpenGLContext::DisableVSync();
                break;
            case Renderer::GraphicsAPI::VULKAN_API:
                // TODO:
                break;
        }
    }
}