//
// Created by kate on 6/4/23.
//

#ifndef KATE_ENGINE_OPENGL_CONTEXT_HH
#define KATE_ENGINE_OPENGL_CONTEXT_HH

#include <any>

#include <Platform/Window/CrossPlatformWindow.hh>
#include <Renderer/RenderContext.hh>

namespace kaTe {
    class OpenGLContext : public RenderContext {
    public:
        explicit OpenGLContext() = default;

        auto Init(std::any windowHandle) -> void override;
        auto ShutDown() -> void override;
        auto SwapBuffers() -> void override;

        ~OpenGLContext() override = default;

    public:
        // Forbidden operations on Contexts
        OpenGLContext(const OpenGLContext&) = delete;
        auto operator=(const OpenGLContext&) -> OpenGLContext& = delete;

        OpenGLContext(OpenGLContext&&) = delete;
        auto operator=(OpenGLContext&&) -> OpenGLContext& = delete;
    private:
        GLFWwindow* m_Handle{};
        bool        m_GLEWInitSuccess{ false };
    };
}


#endif//KATE_ENGINE_OPENGL_CONTEXT_HH
