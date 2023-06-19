/**
 * InputManager.hh
 * Created by kate on 5/30/23.
 * */

#ifndef KATE_ENGINE_INPUT_MANAGER_HH
#define KATE_ENGINE_INPUT_MANAGER_HH

// C++ Standard Library
#include <memory>
#include <utility>

// Project Headers
#include <Tools/Singleton.hh>
#include <Tools/Common.hh>

#include <Core/Assert.hh>

#include <Platform/Window/Window.hh>


namespace kaTe {
    class InputManager {
    public:
        explicit InputManager() = default;
        virtual ~InputManager() = default;

        static auto Init() -> void;

        static auto IsKeyPressed(Int32_T keyCode) -> bool;
        static auto IsMouseKeyPressed(Int32_T button) -> bool;
        static auto GetMouseX() -> double;
        static auto GetMouseY() -> double;
        static auto GetMousePos() -> std::pair<double, double>;

        static auto ShutDown() -> void;

    private:
        inline static std::unique_ptr<InputManager> s_InputManager{};
    protected:
        /* Keyboard Input */
        KT_NODISCARD virtual auto IsKeyPressedNative(Int32_T keyCode) -> bool = 0;

        /* Mouse Input */
        KT_NODISCARD virtual auto IsMouseKeyPressedNative(Int32_T button) -> bool = 0;
        KT_NODISCARD virtual auto GetMouseXNative() -> double = 0;
        KT_NODISCARD virtual auto GetMouseYNative() -> double = 0;
        KT_NODISCARD virtual auto GetMousePosNative() -> std::pair<double, double> = 0;
    };

}


#endif// KATE_ENGINE_INPUT_MANAGER_HH
