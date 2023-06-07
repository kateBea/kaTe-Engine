//
// Created by kate on 6/4/23.
//

#include <any>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "Core/Assert.hh"
#include "Core/EngineManager.hh"
#include "Core/Logger.hh"
#include "Renderer/OpenGL/OpenGLContext.hh"
#include "Tools/Common.hh"

namespace kaTe {

    auto OpenGLContext::init(std::any windowHandle) -> void {
        try {
            // We expect the native window for Linux Window to be a GLFWwindow*
            m_Handle = std::any_cast<GLFWwindow*>(EngineManager::get().getMainWindow().getNativeWindow());
            KT_ASSERT(m_Handle, "OpenGLContext::init() window handle is NULL");
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, KT_OPENGL_VERSION_MAJOR);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, KT_OPENGL_VERSION_MINOR);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

            glfwMakeContextCurrent(m_Handle);
            glewExperimental = GL_TRUE;

            // Using temporal variable because KATE_CORE_LOGGER_ERROR gets stripped
            // in non-DEBUG builds, so glewInit() would not be executed
            m_GLEWInitSuccess = glewInit() == GLEW_OK;
            KT_ASSERT(m_GLEWInitSuccess, "Failed to initialize GLEW");

            KATE_CORE_LOGGER_INFO("OpenGL target  {}.{}", KT_OPENGL_VERSION_MAJOR, KT_OPENGL_VERSION_MINOR);
            KATE_CORE_LOGGER_INFO("OpenGL available {}", (const char*)glGetString(GL_VERSION));
            KATE_CORE_LOGGER_INFO("OpenGL vendor {}", (const char*)glGetString(GL_VENDOR));
            KATE_CORE_LOGGER_INFO("OpenGL renderer {}", (const char*)glGetString(GL_RENDERER));

        }
        catch (const std::bad_any_cast& exception) {
            KATE_APP_LOGGER_ERROR("Exception thrown std::any_cast at OpenGLContext::init. What: {}", exception.what());
        }
    }

    auto OpenGLContext::shutDown() -> void {

    }

    auto OpenGLContext::swapBuffers() -> void {
        glfwSwapBuffers(m_Handle);
    }
}
