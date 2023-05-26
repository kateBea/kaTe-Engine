//
// Created by kate on 5/26/23.
//

#include "../include/Core/Assert.hh"
#include "../include/Core/Logger.hh"

#include "../include/Core/Events/Event.hh"
#include "../include/Core/Events/AppEvents.hh"
#include "../include/Core/Events/KeyEvents.hh"
#include "../include/Core/Events/MouseEvents.hh"

#include "../include/Platform/Window/LinuxWindow.hh"

namespace kT {
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

    LinuxWindow::~LinuxWindow() {
        shutDown();
    }


    auto LinuxWindow::spawn() -> std::unique_ptr<LinuxWindow> {
        KATE_LOGGER_INFO("Spawning new Window");
        return std::make_unique<LinuxWindow>();
    }

    auto LinuxWindow::startUp() -> void {
        KATE_LOGGER_DEBUG("Creating Linux Window with name '{}' and dimension [{}, {}]",
                          m_Data.properties.getName(), m_Data.properties.getWidth(), m_Data.properties.getHeight());

        // Init GLFW
        if (!s_GLFWInitSuccess) {
            auto ret{ glfwInit() };

            if (ret == GLFW_FALSE)
                KT_ASSERT(false && "Failed to initialized the GLFW library");

            s_GLFWInitSuccess = true;

            glfwSetErrorCallback([](std::int32_t errCode, const char* desc) {
                    KATE_LOGGER_ERROR("GLFW error code: {} Description: {}", errCode, desc);
                }
            );
        }

        m_Window = glfwCreateWindow(m_Data.properties.getWidth(), m_Data.properties.getHeight(),
                                    m_Data.properties.getName().c_str(), nullptr, nullptr);
        glfwMakeContextCurrent(m_Window);
        glfwSetWindowUserPointer(m_Window, &m_Data);
        enableVSync();

        // Set Callbacks
        glfwSetWindowSizeCallback(m_Window,
            [](GLFWwindow* window, std::int32_t width, std::int32_t height) {
                WindowData& data{ *static_cast<WindowData*>(glfwGetWindowUserPointer(window)) };
                data.properties.setWidth(width);
                data.properties.setHeight(height);

                WindowResizedEvent wre{ width, height };
                data.callback(wre);
            }
        );

        glfwSetWindowCloseCallback(m_Window,
            [](GLFWwindow* window) {
                WindowData& data{ *static_cast<WindowData*>(glfwGetWindowUserPointer(window)) };
                WindowCloseEvent wce{};
                data.callback(wce);
            }
        );

        glfwSetKeyCallback(m_Window,
            [](GLFWwindow* window, std::int32_t key, std::int32_t scancode, std::int32_t action, std::int32_t mods) {
                WindowData& data{ *static_cast<WindowData*>(glfwGetWindowUserPointer(window)) };

                switch (action) {
                    case GLFW_PRESS: {
                        KeyPressedEvent kpeNoRepeat{ key, false };
                        data.callback(kpeNoRepeat);
                        break;
                    }
                    case GLFW_RELEASE: {
                        KeyReleasedEvent kre{ key };
                        data.callback(kre);
                        break;
                    }
                    case GLFW_REPEAT: {
                        KeyPressedEvent kpeRepeat{ key, true };
                        data.callback(kpeRepeat);
                        break;
                    }
                }
            }
        );

        glfwSetMouseButtonCallback(m_Window,
            [](GLFWwindow* window, std::int32_t button, std::int32_t action, std::int32_t mods) {
                WindowData& data{ *static_cast<WindowData*>(glfwGetWindowUserPointer(window)) };

                switch (action) {
                    case GLFW_PRESS: {
                       MouseButtonPressedEvent mousePressed{ button };
                       data.callback(mousePressed);
                       break;
                    }
                    case GLFW_RELEASE: {
                       MouseButtonReleasedEvent mouseReleased{ button };
                       data.callback(mouseReleased);
                       break;
                    }
                }
            }
        );

        glfwSetScrollCallback(m_Window,
            [](GLFWwindow* window, double xOffset, double yOffset) {
                WindowData& data{ *static_cast<WindowData*>(glfwGetWindowUserPointer(window)) };
                MouseScrollEvent msc{ xOffset, yOffset };
                data.callback(msc);
            }
        );

        glfwSetCursorPosCallback(m_Window,
            [](GLFWwindow* window, double x, double y) {
                WindowData& data{ *static_cast<WindowData*>(glfwGetWindowUserPointer(window)) };
                MouseMovedEvent mme{ x, y };
                data.callback(mme);
            }
        );
    }

    auto LinuxWindow::shutDown() -> void {
        KATE_LOGGER_DEBUG("Shutting down Linux Window with name '{}' and dimension [{}, {}]",
                          m_Data.properties.getName(), m_Data.properties.getWidth(), m_Data.properties.getHeight());
        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }
}