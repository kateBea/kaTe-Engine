/**
 * LinuxWindow.cc
 * Created by kate on 5/26/23.
 * */

// C++ Standard Library
#include <utility>

// Third-Party Libraries


// Projects headers
#include <Core/Assert.hh>
#include <Core/Logger.hh>

#include <Core/Events/Event.hh>
#include <Core/Events/AppEvents.hh>
#include <Core/Events/KeyEvents.hh>
#include <Core/Events/MouseEvents.hh>

#include <Platform/Window/WindowGLFW.hh>
#include <Renderer/OpenGL/OpenGLContext.hh>

namespace kaTe {
    WindowGLFW::WindowGLFW(const WindowProperties& properties)
        :   Window{ properties }, m_Window{ nullptr }, m_Callback{}, m_VSync{ true } {}

    auto WindowGLFW::onUpdate() -> void {
        glfwPollEvents();
        m_Context->swapBuffers();
    }

    auto WindowGLFW::enableVSync() -> void {
        glfwSwapInterval(1);
        m_VSync = true;
    }

    auto WindowGLFW::disableVSync() -> void {
        glfwSwapInterval(0);
        m_VSync = false;
    }

    auto WindowGLFW::init() -> void {
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

    auto WindowGLFW::shutDown() -> void {
        KATE_CORE_LOGGER_DEBUG("Shutting down Linux Window with name '{}' and dimension [{}, {}]",
                               m_Properties.getName(), m_Properties.getWidth(), m_Properties.getHeight());
        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }

    auto WindowGLFW::setCallbacks() -> void {
        glfwSetWindowSizeCallback(m_Window,
            [](GLFWwindow* window, Int32_T width, Int32_T height) {
                WindowGLFW* data{static_cast<WindowGLFW*>(glfwGetWindowUserPointer(window)) };
                data->m_Properties.setWidth(width);
                data->m_Properties.setHeight(height);

                WindowResizedEvent wre{width, height};
                data->m_Callback(wre);
            }
        );

        glfwSetWindowCloseCallback(m_Window,
            [](GLFWwindow* window) {
                WindowGLFW* data{static_cast<WindowGLFW*>(glfwGetWindowUserPointer(window)) };
                WindowCloseEvent wce{};
                data->m_Callback(wce);
            }
        );

        glfwSetKeyCallback(m_Window,
            [](GLFWwindow *window, std::int32_t key, Int32_T scancode, Int32_T action, Int32_T mods) {
                WindowGLFW* data{static_cast<WindowGLFW*>(glfwGetWindowUserPointer(window)) };

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
                WindowGLFW* data{static_cast<WindowGLFW*>(glfwGetWindowUserPointer(window)) };

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
                WindowGLFW* data{static_cast<WindowGLFW*>(glfwGetWindowUserPointer(window)) };
                MouseScrollEvent msc{ xOffset, yOffset };
                data->m_Callback(msc);
            }
        );

        glfwSetCursorPosCallback(m_Window,
            [](GLFWwindow* window, double x, double y) {
                WindowGLFW* data{static_cast<WindowGLFW*>(glfwGetWindowUserPointer(window)) };
                MouseMovedEvent mme{x, y};
                data->m_Callback(mme);
            }
        );

        glfwSetCharCallback(m_Window,
            [](GLFWwindow* window, unsigned int codePoint) {
                WindowGLFW* data{static_cast<WindowGLFW*>(glfwGetWindowUserPointer(window)) };
                KeyCharEvent kce{ codePoint };
                data->m_Callback(kce);
            }
        );
    }
}