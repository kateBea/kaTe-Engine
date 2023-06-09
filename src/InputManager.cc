//
// Created by kate on 6/9/23.
//

#include <utility>

#include <Core/Assert.hh>
#include <Core/Logger.hh>

#include <Platform/InputManager.hh>
#include <Platform/InputManagerGLFW.hh>

namespace kaTe {
    auto InputManager::init() -> void {
        KATE_CORE_LOGGER_INFO("kaTe Engine: Input Manager initialization");
        s_InputManager = new InputManagerGLFW();
    }

    auto InputManager::isKeyPressed(Int32_T keyCode) -> bool {
        KT_ASSERT(s_InputManager, "InputManager singleton ptr is NULL");
        return s_InputManager->isKeyPressedNative(keyCode);
    }

    auto InputManager::isMouseKeyPressed(Int32_T button) -> bool {
        KT_ASSERT(s_InputManager, "InputManager singleton ptr is NULL");
        return s_InputManager->isMouseKeyPressedNative(button);
    }

    auto InputManager::getMouseX() -> double {
        KT_ASSERT(s_InputManager, "InputManager singleton ptr is NULL");
        return s_InputManager->getMouseXNative();
    }

    auto InputManager::getMouseY() -> double {
        KT_ASSERT(s_InputManager, "InputManager singleton ptr is NULL");
        return s_InputManager->getMouseYNative();
    }

    auto InputManager::getMousePos() -> std::pair<double, double> {
        KT_ASSERT(s_InputManager, "InputManager singleton ptr is NULL");
        return s_InputManager->getMousePosNative();
    }

    auto InputManager::shutDown() -> void {
        delete s_InputManager;
    }
}