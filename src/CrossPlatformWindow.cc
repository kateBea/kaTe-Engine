/**
 * LinuxWindow.cc
 * Created by kate on 5/26/23.
 * */

// Third-Party Libraries
#include <GLFW/glfw3.h>

// Projects headers
#include <Core/Logger.hh>
#include <Tools/Common.hh>
#include <Core/Assert.hh>

#include <Core/Events/AppEvents.hh>
#include <Core/Events/KeyEvents.hh>
#include <Core/Events/MouseEvents.hh>

#include <Platform/Window/CrossPlatformWindow.hh>
#include <Renderer/OpenGL/OpenGLContext.hh>
#include <Renderer/RenderContext.hh>

namespace kaTe {
    CrossPlatformWindow::CrossPlatformWindow(const WindowProperties& properties)
        :   Window{ properties }, m_Window{ nullptr }, m_Callback{}, m_VSync{ true } {}

    auto CrossPlatformWindow::OnUpdate() -> void {
        glfwPollEvents();

        // should probably not be here and most likely
        // not be done when the window is minimized, it CPU waste
        m_Context->SwapBuffers();
    }

    auto CrossPlatformWindow::EnableVSync() -> void {
        glfwSwapInterval(1);
        m_VSync = true;
    }

    auto CrossPlatformWindow::DisableVSync() -> void {
        glfwSwapInterval(0);
        m_VSync = false;
    }

    auto CrossPlatformWindow::Init() -> void {
        KATE_CORE_LOGGER_INFO("Main Window initialization");
        InitGLFW();
        KATE_CORE_LOGGER_INFO("Creating Window GLFW. Name '{}'. Dimensions [{}, {}]",
                              m_Properties.GetName(), m_Properties.GetWidth(), m_Properties.GetHeight());

        // Hints
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        m_Window = glfwCreateWindow(m_Properties.GetWidth(), m_Properties.GetHeight(), m_Properties.GetName().c_str(), nullptr, nullptr);
        m_WindowCreateSuccess = m_Window != nullptr;
        KT_ASSERT(m_WindowCreateSuccess, "Failed to create the Window GLFW");

        m_Context = GetActiveAPIContext();
        KT_ASSERT(m_Context, "Graphics Rendering API context is NULL");
        m_Context->Init(GetNativeWindow());

        SpawnOnCenter();
        EnableVSync();
        SetCallbacks();
    }

    auto CrossPlatformWindow::ShutDown() -> void {
        KATE_CORE_LOGGER_DEBUG("Shutting down Window GLFW. Name '{}'. Dimensions [{}, {}]",
                               m_Properties.GetName(), m_Properties.GetWidth(), m_Properties.GetHeight());

        delete m_Context;

        // Might have an internal window counter If we want to spawn multiple windows,
        // so the last one alive shuts down the GLFW library
        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }

    auto CrossPlatformWindow::SetCallbacks() -> void {
        glfwSetWindowUserPointer(m_Window, this);

        glfwSetWindowSizeCallback(m_Window,
            [](GLFWwindow* window, Int32_T width, Int32_T height) {
                                      CrossPlatformWindow * data{static_cast<CrossPlatformWindow*>(glfwGetWindowUserPointer(window)) };
                                      data->m_Properties.SetWidth(width);
                                      data->m_Properties.SetHeight(height);

                WindowResizedEvent wre{width, height};
                data->m_Callback(wre);
            }
        );

        glfwSetWindowCloseCallback(m_Window,
            [](GLFWwindow* window) {
                                       CrossPlatformWindow * data{static_cast<CrossPlatformWindow *>(glfwGetWindowUserPointer(window)) };
                WindowCloseEvent wce{};
                data->m_Callback(wce);
            }
        );

        glfwSetKeyCallback(m_Window,
            [](GLFWwindow *window, std::int32_t key, Int32_T scancode, Int32_T action, Int32_T mods) {
                               CrossPlatformWindow * data{static_cast<CrossPlatformWindow *>(glfwGetWindowUserPointer(window)) };

                switch (action) {
                    case GLFW_PRESS: {
                        KeyPressedEvent kpeNoRepeat{ key, false, mods };
                        data->m_Callback(kpeNoRepeat);
                        break;
                    }
                    case GLFW_RELEASE: {
                        KeyReleasedEvent kre{ key };
                        data->m_Callback(kre);
                        break;
                    }
                    case GLFW_REPEAT: {
                        KeyPressedEvent kpeRepeat{ key, true, mods };
                        data->m_Callback(kpeRepeat);
                        break;
                    }
                }
            }
        );

        glfwSetMouseButtonCallback(m_Window,
            [](GLFWwindow* window, Int32_T button, Int32_T action, Int32_T mods) {
                                       CrossPlatformWindow * data{static_cast<CrossPlatformWindow *>(glfwGetWindowUserPointer(window)) };

                switch (action) {
                    case GLFW_PRESS: {
                        MouseButtonPressedEvent mousePressed{ button, mods };
                        data->m_Callback(mousePressed);
                        break;
                    }
                    case GLFW_RELEASE: {
                        MouseButtonReleasedEvent mouseReleased{ button };
                        data->m_Callback(mouseReleased);
                        break;
                    }
                }
            }
        );

        glfwSetScrollCallback(m_Window,
            [](GLFWwindow* window, double xOffset, double yOffset) {
                                  CrossPlatformWindow * data{static_cast<CrossPlatformWindow *>(glfwGetWindowUserPointer(window)) };
                MouseScrollEvent msc{ xOffset, yOffset };
                data->m_Callback(msc);
            }
        );

        glfwSetCursorPosCallback(m_Window,
            [](GLFWwindow* window, double x, double y) {
                                     CrossPlatformWindow * data{static_cast<CrossPlatformWindow *>(glfwGetWindowUserPointer(window)) };
                MouseMovedEvent mme{x, y};
                data->m_Callback(mme);
            }
        );

        glfwSetCharCallback(m_Window,
            [](GLFWwindow* window, unsigned int codePoint) {
                                CrossPlatformWindow * data{static_cast<CrossPlatformWindow *>(glfwGetWindowUserPointer(window)) };
                KeyCharEvent kce{ codePoint };
                data->m_Callback(kce);
            }
        );
    }
    auto CrossPlatformWindow::SpawnOnCenter() const -> void {
#if defined(_DEBUG) || defined(NDEBUG)
        Int32_T count{};
        GLFWmonitor** monitors{ glfwGetMonitors(&count) };
        KATE_CORE_LOGGER_INFO("Number of available monitors: {}", count);
#endif
        // See: https://www.glfw.org/docs/3.3/monitor_guide.html
        // The primary monitor is returned by glfwGetPrimaryMonitor. It is the user's
        // preferred monitor and is usually the one with global UI elements like task bar or menu bar.
        Int32_T monitorWidth{};
        Int32_T monitorHeight{};
        GLFWmonitor* primary{ glfwGetPrimaryMonitor() };
        glfwGetMonitorWorkarea(primary, nullptr, nullptr, &monitorWidth, &monitorHeight);
        glfwSetWindowPos(m_Window, monitorWidth / 5, monitorHeight / 5);
    }


    auto CrossPlatformWindow::GetActiveAPIContext() -> RenderContext* {
        KATE_CORE_LOGGER_WARN("Default Context is for OpenGL");
        return new OpenGLContext();
    }

    auto CrossPlatformWindow::InitGLFW() -> void {
        if (!g_GLFWInitSuccess) {
            auto ret{ glfwInit() };
            KT_ASSERT(ret == GLFW_TRUE, "Failed to initialized the GLFW library");

            g_GLFWInitSuccess = true;
            glfwSetErrorCallback([](std::int32_t errCode, const char* desc) -> void {
                    KATE_CORE_LOGGER_ERROR("GLFW error code: {} Description: {}", errCode, desc);
                }
            );
        }
    }
}