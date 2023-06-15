/**
 * LinuxInputManager.hh
 * Created by kate on 5/31/23.
 * */

#ifndef KATE_ENGINE_LINUX_INPUT_MANAGER_HH
#define KATE_ENGINE_LINUX_INPUT_MANAGER_HH

// Project Headers
#include "../Tools/Common.hh"
#include "InputManager.hh"
#include "Window/CrossPlatformWindow.hh"
#include "Window/Window.hh"

namespace kaTe {
    class InputManagerGLFW : public InputManager {
    public:
        KT_NODISCARD
        auto isKeyPressedNative(Int32_T keyCode) -> bool override;
        KT_NODISCARD
        auto isMouseKeyPressedNative(Int32_T button) -> bool override;
        KT_NODISCARD
        auto getMouseXNative() -> double override;
        KT_NODISCARD
        auto getMouseYNative() -> double override;
        KT_NODISCARD
        auto getMousePosNative() -> std::pair<double, double> override;
    };

}   // END NAMESPACE kT


#endif//KATE_ENGINE_LINUX_INPUT_MANAGER_HH
