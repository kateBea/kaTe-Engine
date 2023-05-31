/**
 * InputManager.hh
 * Created by kate on 5/30/23.
 * */

#ifndef KATE_ENGINE_INPUT_MANAGER_HH
#define KATE_ENGINE_INPUT_MANAGER_HH

// C++ Standard Library
#include <cstdint>
#include <utility>

// Project Headers
#include <Platform/Window/Window.hh>
#include <kaTe/Singleton.hh>
#include <kaTe/Common.hh>
#include <Core/Assert.hh>

namespace kT {
    class InputManager : public Singleton<InputManager> {
    public:
        using Pos_T = std::pair<double, double>;

        [[nodiscard]]
        inline static auto isKeyPressed(Int32_T keyCode) -> bool {
            KT_ASSERT(getPtr(), "InputManager singleton ptr is NULL");
            return getPtr()->isKeyPressedNative(keyCode);
        }

        [[nodiscard]]
        inline static auto isMouseKeyPressed(Int32_T button) -> bool {
            KT_ASSERT(getPtr(), "InputManager singleton ptr is NULL");
            return getPtr()->isMouseKeyPressedNative(button);
        }

        [[nodiscard]]
        inline static auto getMouseX() -> double {
            KT_ASSERT(getPtr(), "InputManager singleton ptr is NULL");
            return getPtr()->getMouseXNative();
        }

        [[nodiscard]]
        inline static auto getMouseY() -> double {
            KT_ASSERT(getPtr(), "InputManager singleton ptr is NULL");
            return getPtr()->getMouseYNative();
        }

        [[nodiscard]]
        inline static auto getMousePos() -> Pos_T {
            KT_ASSERT(getPtr(), "InputManager singleton ptr is NULL");
            return getPtr()->getMousePosNative();
        }

    protected:
        /* Keyboard Input */
        [[nodiscard]]
        virtual auto isKeyPressedNative(Int32_T keyCode) -> bool = 0;

        /* Mouse Input */
        [[nodiscard]]
        virtual auto isMouseKeyPressedNative(Int32_T button) -> bool = 0;
        [[nodiscard]]
        virtual auto getMouseXNative() -> double = 0;
        [[nodiscard]]
        virtual auto getMouseYNative() -> double = 0;
        [[nodiscard]]
        virtual auto getMousePosNative() -> Pos_T = 0;
    };

}


#endif// KATE_ENGINE_INPUT_MANAGER_HH
