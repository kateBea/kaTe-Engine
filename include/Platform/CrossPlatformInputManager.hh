/**
 * LinuxInputManager.hh
 * Created by kate on 5/31/23.
 * */

#ifndef KATE_ENGINE_LINUX_INPUT_MANAGER_HH
#define KATE_ENGINE_LINUX_INPUT_MANAGER_HH

// Project Headers
#include <Tools/Common.hh>
#include <Platform/InputManager.hh>
#include <Platform/Window/CrossPlatformWindow.hh>
#include <Platform/Window/Window.hh>

namespace kaTe {
    class CrossPlatformInputManager : public InputManager {
    public:
        KT_NODISCARD auto IsKeyPressedNative(Int32_T keyCode) -> bool override;
        KT_NODISCARD auto IsMouseKeyPressedNative(Int32_T button) -> bool override;
        KT_NODISCARD auto GetMouseXNative() -> double override;
        KT_NODISCARD auto GetMouseYNative() -> double override;
        KT_NODISCARD auto GetMousePosNative() -> std::pair<double, double> override;
    };

}   // END NAMESPACE kaTe


#endif//KATE_ENGINE_LINUX_INPUT_MANAGER_HH
