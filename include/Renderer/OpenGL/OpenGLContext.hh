//
// Created by kate on 6/4/23.
//

#ifndef KATE_ENGINE_OPENGL_CONTEXT_HH
#define KATE_ENGINE_OPENGL_CONTEXT_HH

#include <any>

#include "Platform/Window/kaTeGLFWwindow.hh"
#include "Renderer/RenderContext.hh"

namespace kaTe {
    class OpenGLContext : public RenderContext {
    public:
        OpenGLContext() = default;

        auto init(std::any handle) -> void;
        auto shutDown() -> void override;
        auto swapBuffers() -> void override;

        ~OpenGLContext() = default;

    private:
        // Forbidden operations on Contexts
        OpenGLContext(const OpenGLContext&) = delete;
        auto operator=(const OpenGLContext&) -> OpenGLContext& = delete;

        OpenGLContext(OpenGLContext&&) = delete;
        auto operator=(OpenGLContext&&) -> OpenGLContext& = delete;
    private:
        // We may want to have various OpenGL contexts
        bool m_GLEWInitSuccess{ false };
        GLFWwindow* m_Handle{};
    };
}


#endif//KATE_ENGINE_OPENGL_CONTEXT_HH
