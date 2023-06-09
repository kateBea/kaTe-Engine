/**
 * InputManager.hh
 * Created by kate on 5/30/23.
 * */

#ifndef KATE_ENGINE_INPUT_MANAGER_HH
#define KATE_ENGINE_INPUT_MANAGER_HH

// C++ Standard Library
#include <utility>

// Project Headers
#include <Core/Assert.hh>

#include <Platform/Window/Window.hh>

#include <Tools/Singleton.hh>
#include <Tools/Common.hh>

namespace kaTe {
    class InputManager {
    public:
        static auto init() -> void;

        static auto isKeyPressed(Int32_T keyCode) -> bool;
        static auto isMouseKeyPressed(Int32_T button) -> bool;
        static auto getMouseX() -> double;
        static auto getMouseY() -> double;
        static auto getMousePos() -> std::pair<double, double>;

        static auto shutDown() -> void;

    private:
        inline static InputManager* s_InputManager{};
    protected:
        /* Keyboard Input */
        KT_NODISCARD
        virtual auto isKeyPressedNative(Int32_T keyCode) -> bool = 0;

        /* Mouse Input */
        KT_NODISCARD
        virtual auto isMouseKeyPressedNative(Int32_T button) -> bool = 0;
        KT_NODISCARD
        virtual auto getMouseXNative() -> double = 0;
        KT_NODISCARD
        virtual auto getMouseYNative() -> double = 0;
        KT_NODISCARD
        virtual auto getMousePosNative() -> std::pair<double, double> = 0;
    };

}


#endif// KATE_ENGINE_INPUT_MANAGER_HH
