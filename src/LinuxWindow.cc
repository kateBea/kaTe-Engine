//
// Created by kate on 5/26/23.
//

#include "../include/Platform/Window/LinuxWindow.hh"
#include "../include/Core/Assert.hh"
#include "../include/Core/Logger.hh"

namespace kT {
    auto LinuxWindow::onUpdate() -> void {
        glfwPollEvents();
        glfwSwapBuffers(m_Window);
    }

    auto LinuxWindow::setEventCallback(EventCallbackFunc func) -> void {

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
        }

        m_Window = glfwCreateWindow(m_Data.properties.getWidth(), m_Data.properties.getHeight(),
                                    m_Data.properties.getName().c_str(), nullptr, nullptr);
        glfwMakeContextCurrent(m_Window);
        glfwSetWindowUserPointer(m_Window, &m_Data);
        enableVSync();
    }

    auto LinuxWindow::shutDown() -> void {
        KATE_LOGGER_DEBUG("Shutting down Linux Window with name '{}' and dimension [{}, {}]",
                          m_Data.properties.getName(), m_Data.properties.getWidth(), m_Data.properties.getHeight());
        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }
}