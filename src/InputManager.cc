//
// Created by kate on 6/9/23.
//

#include <memory>
#include <utility>

#include <Core/Assert.hh>
#include <Core/Logger.hh>

#include <Platform/CrossPlatformInputManager.hh>
#include <Platform/InputManager.hh>

namespace kaTe {
    auto InputManager::Init() -> void {
        KATE_CORE_LOGGER_INFO("kaTe Engine: Input Manager initialization");
        s_InputManager = std::make_unique<CrossPlatformInputManager>();
    }

    auto InputManager::IsKeyPressed(Int32_T keyCode) -> bool {
        KT_ASSERT(s_InputManager, "InputManager singleton ptr is NULL");
        return s_InputManager->IsKeyPressedNative(keyCode);
    }

    auto InputManager::IsMouseKeyPressed(Int32_T button) -> bool {
        KT_ASSERT(s_InputManager, "InputManager singleton ptr is NULL");
        return s_InputManager->IsMouseKeyPressedNative(button);
    }

    auto InputManager::GetMouseX() -> double {
        KT_ASSERT(s_InputManager, "InputManager singleton ptr is NULL");
        return s_InputManager->GetMouseXNative();
    }

    auto InputManager::GetMouseY() -> double {
        KT_ASSERT(s_InputManager, "InputManager singleton ptr is NULL");
        return s_InputManager->GetMouseYNative();
    }

    auto InputManager::GetMousePos() -> std::pair<double, double> {
        KT_ASSERT(s_InputManager, "InputManager singleton ptr is NULL");
        return s_InputManager->GetMousePosNative();
    }

    auto InputManager::ShutDown() -> void {

    }
}