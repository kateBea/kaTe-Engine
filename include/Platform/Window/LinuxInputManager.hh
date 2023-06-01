/**
 * LinuxInputManager.hh
 * Created by kate on 5/31/23.
 * */

#ifndef KATE_ENGINE_LINUX_INPUT_MANAGER_HH
#define KATE_ENGINE_LINUX_INPUT_MANAGER_HH

// Project Headers
#include <Platform/Window/InputManager.hh>
#include <Platform/Window/Window.hh>
#include <Platform/Window/LinuxWindow.hh>

namespace kaTe {
    class LinuxInputManager : public InputManager {
    public:
        [[nodiscard]]
        auto isKeyPressedNative(Int32_T keyCode) -> bool override;
        [[nodiscard]]
        auto isMouseKeyPressedNative(Int32_T button) -> bool override;
        [[nodiscard]]
        auto getMouseXNative() -> double override;
        [[nodiscard]]
        auto getMouseYNative() -> double override;
        [[nodiscard]]
        auto getMousePosNative() -> Pos_T override;
    };

}   // END NAMESPACE kT


#endif//KATE_ENGINE_LINUX_INPUT_MANAGER_HH
