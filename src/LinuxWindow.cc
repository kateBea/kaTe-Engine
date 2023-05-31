/**
 * LinuxWindow.cc
 * Created by kate on 5/26/23.
 * */

// C++ Standard Library
#include <utility>

// Third-Party Libraries
#include <GL/glew.h>

// Projects headers
#include "../include/Core/Assert.hh"
#include "../include/Core/Logger.hh"

#include "../include/Core/Events/Event.hh"
#include "../include/Core/Events/AppEvents.hh"
#include "../include/Core/Events/KeyEvents.hh"
#include "../include/Core/Events/MouseEvents.hh"

#include "../include/Platform/Window/LinuxWindow.hh"

namespace kT {
    LinuxWindow::LinuxWindow(const WindowProperties& properties)
        :   Window{}, m_Data{ .properties{ properties }, .callback{}, .VSync{ true } }, m_Window{ nullptr }
    {}

    LinuxWindow::LinuxWindow(LinuxWindow&& other)
        :   Window{ std::move(other) }, m_Data{ std::move(other.m_Data) }, m_Window{ other.m_Window }
    {
        // Invalidate other
        other.m_Window = nullptr;
    }

    auto LinuxWindow::operator=(LinuxWindow&& other) noexcept -> LinuxWindow& {
        Window::operator=(std::move(other));
        m_Data = std::move(other.m_Data);
        m_Window = other.m_Window;

        // Invalidate other
        other.m_Window = nullptr;

        return *this;
    }

    auto LinuxWindow::onUpdate() -> void {
        glfwPollEvents();
        glfwSwapBuffers(m_Window);
    }

    auto LinuxWindow::enableVSync() -> void {
        glfwSwapInterval(1);
        m_Data.VSync = true;
    }

    auto LinuxWindow::disableVSync() -> void {
        glfwSwapInterval(0);
        m_Data.VSync = false;
    }

    auto LinuxWindow::init() -> void {
        KATE_CORE_LOGGER_DEBUG("Creating Linux_Window with name '{}' and dimension [{}, {}]",
                               m_Data.properties.getName(), m_Data.properties.getWidth(), m_Data.properties.getHeight());
        // Init GLFW
        if (!g_GLFWInitSuccess) {
            auto ret{ glfwInit() };
            KT_ASSERT(ret == GLFW_TRUE, "Failed to initialized the GLFW library");

            g_GLFWInitSuccess = true;
            glfwSetErrorCallback([](std::int32_t errCode, const char* desc) -> void {
                    KATE_CORE_LOGGER_ERROR("GLFW error code: {} Description: {}", errCode, desc);
                }
            );

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, KT_OPENGL_VERSION_MAJOR);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, KT_OPENGL_VERSION_MINOR);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        }

        m_Window = glfwCreateWindow(m_Data.properties.getWidth(), m_Data.properties.getHeight(),
                                    m_Data.properties.getName().c_str(), nullptr, nullptr);
        m_WindowCreateSuccess = m_Window != nullptr;
        KT_ASSERT(m_WindowCreateSuccess, "Failed to create the Linux_Window");

        glfwMakeContextCurrent(m_Window);

        // Init GLEW
        glewExperimental = GL_TRUE;
        // Using temporal variable because KATE_CORE_LOGGER_ERROR gets stripped
        // in non-DEBUG builds, so glewInit() would not be executed
        auto ret{ glewInit() == GLEW_OK };
        KT_ASSERT(ret, "Failed to initialize GLEW");

        glfwSetWindowUserPointer(m_Window, &m_Data);
        enableVSync();
        setCallbacks();
    }

    auto LinuxWindow::shutDown() -> void {
        KATE_CORE_LOGGER_DEBUG("Shutting down Linux Window with name '{}' and dimension [{}, {}]",
                               m_Data.properties.getName(), m_Data.properties.getWidth(), m_Data.properties.getHeight());
        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }

    auto LinuxWindow::setCallbacks() -> void {
        glfwSetWindowSizeCallback(m_Window,
            [](GLFWwindow *window, std::int32_t width, std::int32_t height) {
                WindowData &data{*static_cast<WindowData *>(glfwGetWindowUserPointer(window))};
                data.properties.setWidth(width);
                data.properties.setHeight(height);

                WindowResizedEvent wre{width, height};
                data.callback(wre);
            }
        );

        glfwSetWindowCloseCallback(m_Window,
            [](GLFWwindow *window) {
                WindowData &data{*static_cast<WindowData *>(glfwGetWindowUserPointer(window))};
                WindowCloseEvent wce{};
                data.callback(wce);
            }
        );

        glfwSetKeyCallback(m_Window,
            [](GLFWwindow *window, std::int32_t key, std::int32_t scancode, std::int32_t action, std::int32_t mods) {
                WindowData &data{*static_cast<WindowData *>(glfwGetWindowUserPointer(window))};

                switch (action) {
                    case GLFW_PRESS: {
                        KeyPressedEvent kpeNoRepeat{key, false};
                        data.callback(kpeNoRepeat);
                        break;
                    }
                    case GLFW_RELEASE: {
                        KeyReleasedEvent kre{key};
                        data.callback(kre);
                        break;
                    }
                    case GLFW_REPEAT: {
                        KeyPressedEvent kpeRepeat{key, true};
                        data.callback(kpeRepeat);
                        break;
                    }
                }
            }
        );

        glfwSetMouseButtonCallback(m_Window,
            [](GLFWwindow *window, std::int32_t button, std::int32_t action, std::int32_t mods) {
                WindowData &data{*static_cast<WindowData *>(glfwGetWindowUserPointer(window))};

                switch (action) {
                    case GLFW_PRESS: {
                        MouseButtonPressedEvent mousePressed{button};
                        data.callback(mousePressed);
                        break;
                    }
                    case GLFW_RELEASE: {
                        MouseButtonReleasedEvent mouseReleased{button};
                        data.callback(mouseReleased);
                        break;
                    }
                }
            }
        );

        glfwSetScrollCallback(m_Window,
            [](GLFWwindow* window, double xOffset, double yOffset) {
                WindowData &data{ *static_cast<WindowData*>(glfwGetWindowUserPointer(window)) };
                MouseScrollEvent msc{ xOffset, yOffset };
                data.callback(msc);
            }
        );

        glfwSetCursorPosCallback(m_Window,
            [](GLFWwindow* window, double x, double y) {
                WindowData &data{ *static_cast<WindowData*>(glfwGetWindowUserPointer(window)) };
                MouseMovedEvent mme{x, y};
                data.callback(mme);
            }
        );

        glfwSetCharCallback(m_Window,
            [](GLFWwindow* window, unsigned int codePoint) {
                WindowData &data{ *static_cast<WindowData*>(glfwGetWindowUserPointer(window)) };
                KeyCharEvent kce{ codePoint };
                data.callback(kce);
            }
        );
    }
}