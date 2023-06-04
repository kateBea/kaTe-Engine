/**
 * LinuxWindow.cc
 * Created by kate on 5/26/23.
 * */

// C++ Standard Library
#include <utility>

// Third-Party Libraries
#include <GL/glew.h>

// Projects headers
#include <Core/Assert.hh>
#include <Core/Logger.hh>

#include <Core/Events/Event.hh>
#include <Core/Events/AppEvents.hh>
#include <Core/Events/KeyEvents.hh>
#include <Core/Events/MouseEvents.hh>

#include <Platform/Window/kaTeGLFWwindow.hh>
#include <Renderer/OpenGL/OpenGLContext.hh>

namespace kaTe {
    kateGLFWwindow::kateGLFWwindow(const WindowProperties& properties)
        :   Window{}, m_Window{ nullptr }, m_Callback{}, m_VSync{ true } {}

    auto kateGLFWwindow::onUpdate() -> void {
        glfwPollEvents();
        m_Context->swapBuffers();
    }

    auto kateGLFWwindow::enableVSync() -> void {
        glfwSwapInterval(1);
        m_VSync = true;
    }

    auto kateGLFWwindow::disableVSync() -> void {
        glfwSwapInterval(0);
        m_VSync = false;
    }

    auto kateGLFWwindow::init() -> void {
        KATE_CORE_LOGGER_DEBUG("Creating Linux_Window with name '{}' and dimension [{}, {}]",
                               m_Properties.getName(), m_Properties.getWidth(), m_Properties.getHeight());
        // Init GLFW
        if (!g_GLFWInitSuccess) {
            auto ret{ glfwInit() };
            KT_ASSERT(ret == GLFW_TRUE, "Failed to initialized the GLFW library");

            g_GLFWInitSuccess = true;
            glfwSetErrorCallback([](std::int32_t errCode, const char* desc) -> void {
                    KATE_CORE_LOGGER_ERROR("GLFW error code: {} Description: {}", errCode, desc);
                }
            );

            glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        }

        m_Window = glfwCreateWindow(m_Properties.getWidth(), m_Properties.getHeight(), m_Properties.getName().c_str(), nullptr, nullptr);
        m_WindowCreateSuccess = m_Window != nullptr;
        KT_ASSERT(m_WindowCreateSuccess, "Failed to create the Linux_Window");

        m_Context = new OpenGLContext();
        m_Context->init(getNativeWindow());

        glfwSetWindowUserPointer(m_Window, this);
        enableVSync();
        setCallbacks();
    }

    auto kateGLFWwindow::shutDown() -> void {
        KATE_CORE_LOGGER_DEBUG("Shutting down Linux Window with name '{}' and dimension [{}, {}]",
                               m_Properties.getName(), m_Properties.getWidth(), m_Properties.getHeight());
        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }

    auto kateGLFWwindow::setCallbacks() -> void {
        glfwSetWindowSizeCallback(m_Window,
            [](GLFWwindow* window, Int32_T width, Int32_T height) {
                kateGLFWwindow* data{ static_cast<kateGLFWwindow*>(glfwGetWindowUserPointer(window)) };
                data->m_Properties.setWidth(width);
                data->m_Properties.setHeight(height);

                WindowResizedEvent wre{width, height};
                data->m_Callback(wre);
            }
        );

        glfwSetWindowCloseCallback(m_Window,
            [](GLFWwindow* window) {
                kateGLFWwindow* data{ static_cast<kateGLFWwindow*>(glfwGetWindowUserPointer(window)) };
                WindowCloseEvent wce{};
                data->m_Callback(wce);
            }
        );

        glfwSetKeyCallback(m_Window,
            [](GLFWwindow *window, std::int32_t key, Int32_T scancode, Int32_T action, Int32_T mods) {
                kateGLFWwindow* data{ static_cast<kateGLFWwindow*>(glfwGetWindowUserPointer(window)) };

                switch (action) {
                    case GLFW_PRESS: {
                        KeyPressedEvent kpeNoRepeat{key, false};
                        data->m_Callback(kpeNoRepeat);
                        break;
                    }
                    case GLFW_RELEASE: {
                        KeyReleasedEvent kre{key};
                        data->m_Callback(kre);
                        break;
                    }
                    case GLFW_REPEAT: {
                        KeyPressedEvent kpeRepeat{key, true};
                        data->m_Callback(kpeRepeat);
                        break;
                    }
                }
            }
        );

        glfwSetMouseButtonCallback(m_Window,
            [](GLFWwindow* window, Int32_T button, Int32_T action, Int32_T mods) {
                kateGLFWwindow* data{ static_cast<kateGLFWwindow*>(glfwGetWindowUserPointer(window)) };

                switch (action) {
                    case GLFW_PRESS: {
                        MouseButtonPressedEvent mousePressed{button};
                        data->m_Callback(mousePressed);
                        break;
                    }
                    case GLFW_RELEASE: {
                        MouseButtonReleasedEvent mouseReleased{button};
                        data->m_Callback(mouseReleased);
                        break;
                    }
                }
            }
        );

        glfwSetScrollCallback(m_Window,
            [](GLFWwindow* window, double xOffset, double yOffset) {
                kateGLFWwindow* data{ static_cast<kateGLFWwindow*>(glfwGetWindowUserPointer(window)) };
                MouseScrollEvent msc{ xOffset, yOffset };
                data->m_Callback(msc);
            }
        );

        glfwSetCursorPosCallback(m_Window,
            [](GLFWwindow* window, double x, double y) {
                kateGLFWwindow* data{ static_cast<kateGLFWwindow*>(glfwGetWindowUserPointer(window)) };
                MouseMovedEvent mme{x, y};
                data->m_Callback(mme);
            }
        );

        glfwSetCharCallback(m_Window,
            [](GLFWwindow* window, unsigned int codePoint) {
                kateGLFWwindow* data{ static_cast<kateGLFWwindow*>(glfwGetWindowUserPointer(window)) };
                KeyCharEvent kce{ codePoint };
                data->m_Callback(kce);
            }
        );
    }
}