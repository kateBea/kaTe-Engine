/**
* LinuxInputManager.hh
* Created by kate on 5/31/23.
* */

// C++ Standard Library
#include <any>
#include <utility>

// Third-Party Libraries
#include <GLFW/glfw3.h>

// Project Headers
#include "Core/Application.hh"
#include <Tools/Common.hh>

#include <Core/Logger.hh>
#include "Platform/InputManagerGLFW.hh"

namespace kaTe {
    auto InputManagerGLFW::isKeyPressedNative(Int32_T keyCode) -> bool {
        GLFWwindow* window{ nullptr };

        try {
            // We expect the native window for Linux Window to be a GLFWwindow*
            window = std::any_cast<GLFWwindow*>(Application::Get().GetMainWindow().getNativeWindow());
            Int32_T state{ glfwGetKey(window, keyCode) };

            return state == GLFW_PRESS || state == GLFW_REPEAT;
        }
        catch (const std::bad_any_cast& exception) {
            KATE_APP_LOGGER_ERROR("Exception thrown std::any_cast. What: {}", exception.what());
        }

        return false;
    }

    auto InputManagerGLFW::isMouseKeyPressedNative(Int32_T button) -> bool {
        GLFWwindow* window{ nullptr };

        try {
            // We expect the native window for Linux Window to be a GLFWwindow*
            window = std::any_cast<GLFWwindow*>(Application::Get().GetMainWindow().getNativeWindow());
            Int32_T state{ glfwGetMouseButton(window, button) };

            return state == GLFW_PRESS || state == GLFW_REPEAT;
        }
        catch (const std::bad_any_cast& exception) {
            KATE_APP_LOGGER_ERROR("Exception thrown std::any_cast. What: {}", exception.what());
        }

        return false;
    }

    auto InputManagerGLFW::getMousePosNative() -> std::pair<double, double> {
        double posX{};
        double posY{};
        GLFWwindow* window{ nullptr };

        try {
            // We expect the native window for Linux Window to be a GLFWwindow*
            window = std::any_cast<GLFWwindow*>(Application::Get().GetMainWindow().getNativeWindow());
            glfwGetCursorPos(window, &posX, &posY);
        }
        catch (const std::bad_any_cast& exception) {
            KATE_APP_LOGGER_ERROR("Exception thrown std::any_cast. What: {}", exception.what());
        }

        return std::make_pair(posX, posY);
    }

    auto InputManagerGLFW::getMouseXNative() -> double {
        auto [mouseX, mouseY]{ getMousePosNative() };
        return mouseX;
    }

    auto InputManagerGLFW::getMouseYNative() -> double {
        auto [mouseX, mouseY]{ getMousePosNative() };
        return mouseY;
    }
}
