/**
 * ImGuiLayer.cc
 * Created by kate on 5/28/23.
 * */

// C++ Standard Library
#include <any>

// Third-Party Libraries
#include <imgui.h>

// The headers in this section shouldn't be here most likely
// this header probably shouldn't be here
// used for ImGuiContext& g = *GImGui;
#define IMGUI_INTERNAL_SHOULD_PROB_NOT_BE_HERE
#include <imgui_internal.h>
#include <GL/glew.h>

// Project Headers
#include <Core/Logger.hh>
#include <kaTe/Application.hh>
#include <Core/KeyCodes.hh>


#include <Core/Layers/ImGuiLayer.hh>
#include <Platform/OpenGL/ImGuiOpenGLRenderer.hh>
#include <kaTe/Common.hh>
#include <Platform/Window/InputManager.hh>

namespace kT {
    ImGuiLayer::ImGuiLayer() noexcept
        :   Layer{ "ImGuiLayer" }
    {
        // Empty for now
    }

    auto ImGuiLayer::onAttach() -> void {
        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        ImGuiIO& io = ImGui::GetIO();

        // Backend capabilities
        io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
        io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

        // Load custom font
        constexpr auto fontSize{ 15 };
        io.Fonts->AddFontFromFileTTF("../assets/Fonts/Inter/Inter-VariableFont.ttf", fontSize);

        ImGui_ImplOpenGL3_Init("#version 410");
    }

    auto ImGuiLayer::onDetach() -> void {
        ImGui_ImplOpenGL3_Shutdown();
    }

    auto ImGuiLayer::onUpdate() -> void {
        ImGuiIO io{ ImGui::GetIO() };
        Application& app{ Application::get() };

        auto height{ app.getWindow().getHeight() };
        auto width{ app.getWindow().getWidth()};
        //io.DisplaySize = ImVec2(width, height);

#if defined(IMGUI_INTERNAL_SHOULD_PROB_NOT_BE_HERE)
        ImGuiContext& g = *GImGui;

        g.IO.DisplaySize.x = width;
        g.IO.DisplaySize.y = height;
#endif

        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        {
            bool bl{ true };
            ImGui::ShowDemoWindow(&bl);

            ImGui::Begin("Test");
            ImGui::SetWindowSize("Test", { 128, 64 });
            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    auto ImGuiLayer::onEvent(Event& event) -> void {
        EventDispatcher dispatcher{ event };

        if (dispatcher.forward<MouseMovedEvent>(KT_BIND_EVENT_FUNC(ImGuiLayer::onMouseMovedEvent))) {}
        if (dispatcher.forward<MouseButtonPressedEvent>(KT_BIND_EVENT_FUNC(ImGuiLayer::onMouseButtonPressedEvent))) {}
        if (dispatcher.forward<MouseButtonReleasedEvent>(KT_BIND_EVENT_FUNC(ImGuiLayer::onMouseButtonReleasedEvent))) {}
        if (dispatcher.forward<MouseScrollEvent>(KT_BIND_EVENT_FUNC(ImGuiLayer::onMouseScrollEvent))) {}
        if (dispatcher.forward<KeyPressedEvent>(KT_BIND_EVENT_FUNC(ImGuiLayer::onKeyPressedEvent))) {}
        if (dispatcher.forward<KeyReleasedEvent>(KT_BIND_EVENT_FUNC(ImGuiLayer::onKeyReleasedEvent))) {}
        if (dispatcher.forward<KeyCharEvent>(KT_BIND_EVENT_FUNC(ImGuiLayer::onKeyTypedEvent))) {}
        if (dispatcher.forward<WindowResizedEvent>(KT_BIND_EVENT_FUNC(ImGuiLayer::onWindowResizedEvent))) {}
    }

    auto ImGuiLayer::onMouseMovedEvent(MouseMovedEvent& event) -> bool {
        ImGuiIO& io{ ImGui::GetIO() };
        io.MousePos = ImVec2{ static_cast<float>(event.getPositionX()),
                             static_cast<float>(event.getPositionY()) };

        // In cse we want to propagate this event
        // we say it is not handled
        return false;
    }

    auto ImGuiLayer::onMouseButtonPressedEvent(MouseButtonPressedEvent& event) -> bool {
        ImGuiIO& io{ ImGui::GetIO() };
        io.MouseDown[event.getMouseButton()] = true;

        // In cse we want to propagate this event
        // we say it is not handled
        return false;
    }

    auto ImGuiLayer::onMouseButtonReleasedEvent(MouseButtonReleasedEvent& event) -> bool {
        ImGuiIO& io{ ImGui::GetIO() };
        io.MouseDown[event.getMouseButton()] = false;

        // In cse we want to propagate this event
        // we say it is not handled
        return false;
    }

    auto ImGuiLayer::onMouseScrollEvent(MouseScrollEvent& event) -> bool {
        ImGuiIO& io{ ImGui::GetIO() };

        io.MouseWheel += event.getOffsetY();
        io.MouseWheelH += event.getOffsetX();

        // In cse we want to propagate this event
        // we say it is not handled
        return false;
    }

    auto ImGuiLayer::ImGuiToKtKey(Int32_T key) const -> ImGuiKey {
        switch (key) {
            case KT_KEY_TAB:
                return ImGuiKey_Tab;
            case KT_KEY_LEFT:
                return ImGuiKey_LeftArrow;
            case KT_KEY_RIGHT:
                return ImGuiKey_RightArrow;
            case KT_KEY_UP:
                return ImGuiKey_UpArrow;
            case KT_KEY_DOWN:
                return ImGuiKey_DownArrow;
            case KT_KEY_PAGE_UP:
                return ImGuiKey_PageUp;
            case KT_KEY_PAGE_DOWN:
                return ImGuiKey_PageDown;
            case KT_KEY_HOME:
                return ImGuiKey_Home;
            case KT_KEY_END:
                return ImGuiKey_End;
            case KT_KEY_INSERT:
                return ImGuiKey_Insert;
            case KT_KEY_DELETE:
                return ImGuiKey_Delete;
            case KT_KEY_BACKSPACE:
                return ImGuiKey_Backspace;
            case KT_KEY_SPACE:
                return ImGuiKey_Space;
            case KT_KEY_ENTER:
                return ImGuiKey_Enter;
            case KT_KEY_ESCAPE:
                return ImGuiKey_Escape;
            case KT_KEY_APOSTROPHE:
                return ImGuiKey_Apostrophe;
            case KT_KEY_COMMA:
                return ImGuiKey_Comma;
            case KT_KEY_MINUS:
                return ImGuiKey_Minus;
            case KT_KEY_PERIOD:
                return ImGuiKey_Period;
            case KT_KEY_SLASH:
                return ImGuiKey_Slash;
            case KT_KEY_SEMICOLON:
                return ImGuiKey_Semicolon;
            case KT_KEY_EQUAL:
                return ImGuiKey_Equal;
            case KT_KEY_LEFT_BRACKET:
                return ImGuiKey_LeftBracket;
            case KT_KEY_BACKSLASH:
                return ImGuiKey_Backslash;
            case KT_KEY_RIGHT_BRACKET:
                return ImGuiKey_RightBracket;
            case KT_KEY_GRAVE_ACCENT:
                return ImGuiKey_GraveAccent;
            case KT_KEY_CAPS_LOCK:
                return ImGuiKey_CapsLock;
            case KT_KEY_SCROLL_LOCK:
                return ImGuiKey_ScrollLock;
            case KT_KEY_NUM_LOCK:
                return ImGuiKey_NumLock;
            case KT_KEY_PRINT_SCREEN:
                return ImGuiKey_PrintScreen;
            case KT_KEY_PAUSE:
                return ImGuiKey_Pause;
            case KT_KEY_KP_0:
                return ImGuiKey_Keypad0;
            case KT_KEY_KP_1:
                return ImGuiKey_Keypad1;
            case KT_KEY_KP_2:
                return ImGuiKey_Keypad2;
            case KT_KEY_KP_3:
                return ImGuiKey_Keypad3;
            case KT_KEY_KP_4:
                return ImGuiKey_Keypad4;
            case KT_KEY_KP_5:
                return ImGuiKey_Keypad5;
            case KT_KEY_KP_6:
                return ImGuiKey_Keypad6;
            case KT_KEY_KP_7:
                return ImGuiKey_Keypad7;
            case KT_KEY_KP_8:
                return ImGuiKey_Keypad8;
            case KT_KEY_KP_9:
                return ImGuiKey_Keypad9;
            case KT_KEY_KP_DECIMAL:
                return ImGuiKey_KeypadDecimal;
            case KT_KEY_KP_DIVIDE:
                return ImGuiKey_KeypadDivide;
            case KT_KEY_KP_MULTIPLY:
                return ImGuiKey_KeypadMultiply;
            case KT_KEY_KP_SUBTRACT:
                return ImGuiKey_KeypadSubtract;
            case KT_KEY_KP_ADD:
                return ImGuiKey_KeypadAdd;
            case KT_KEY_KP_ENTER:
                return ImGuiKey_KeypadEnter;
            case KT_KEY_KP_EQUAL:
                return ImGuiKey_KeypadEqual;
            case KT_KEY_LEFT_SHIFT:
                return ImGuiKey_LeftShift;
            case KT_KEY_LEFT_CONTROL:
                return ImGuiKey_LeftCtrl;
            case KT_KEY_LEFT_ALT:
                return ImGuiKey_LeftAlt;
            case KT_KEY_LEFT_SUPER:
                return ImGuiKey_LeftSuper;
            case KT_KEY_RIGHT_SHIFT:
                return ImGuiKey_RightShift;
            case KT_KEY_RIGHT_CONTROL:
                return ImGuiKey_RightCtrl;
            case KT_KEY_RIGHT_ALT:
                return ImGuiKey_RightAlt;
            case KT_KEY_RIGHT_SUPER:
                return ImGuiKey_RightSuper;
            case KT_KEY_MENU:
                return ImGuiKey_Menu;
            case KT_KEY_0:
                return ImGuiKey_0;
            case KT_KEY_1:
                return ImGuiKey_1;
            case KT_KEY_2:
                return ImGuiKey_2;
            case KT_KEY_3:
                return ImGuiKey_3;
            case KT_KEY_4:
                return ImGuiKey_4;
            case KT_KEY_5:
                return ImGuiKey_5;
            case KT_KEY_6:
                return ImGuiKey_6;
            case KT_KEY_7:
                return ImGuiKey_7;
            case KT_KEY_8:
                return ImGuiKey_8;
            case KT_KEY_9:
                return ImGuiKey_9;
            case KT_KEY_A:
                return ImGuiKey_A;
            case KT_KEY_B:
                return ImGuiKey_B;
            case KT_KEY_C:
                return ImGuiKey_C;
            case KT_KEY_D:
                return ImGuiKey_D;
            case KT_KEY_E:
                return ImGuiKey_E;
            case KT_KEY_F:
                return ImGuiKey_F;
            case KT_KEY_G:
                return ImGuiKey_G;
            case KT_KEY_H:
                return ImGuiKey_H;
            case KT_KEY_I:
                return ImGuiKey_I;
            case KT_KEY_J:
                return ImGuiKey_J;
            case KT_KEY_K:
                return ImGuiKey_K;
            case KT_KEY_L:
                return ImGuiKey_L;
            case KT_KEY_M:
                return ImGuiKey_M;
            case KT_KEY_N:
                return ImGuiKey_N;
            case KT_KEY_O:
                return ImGuiKey_O;
            case KT_KEY_P:
                return ImGuiKey_P;
            case KT_KEY_Q:
                return ImGuiKey_Q;
            case KT_KEY_R:
                return ImGuiKey_R;
            case KT_KEY_S:
                return ImGuiKey_S;
            case KT_KEY_T:
                return ImGuiKey_T;
            case KT_KEY_U:
                return ImGuiKey_U;
            case KT_KEY_V:
                return ImGuiKey_V;
            case KT_KEY_W:
                return ImGuiKey_W;
            case KT_KEY_X:
                return ImGuiKey_X;
            case KT_KEY_Y:
                return ImGuiKey_Y;
            case KT_KEY_Z:
                return ImGuiKey_Z;
            case KT_KEY_F1:
                return ImGuiKey_F1;
            case KT_KEY_F2:
                return ImGuiKey_F2;
            case KT_KEY_F3:
                return ImGuiKey_F3;
            case KT_KEY_F4:
                return ImGuiKey_F4;
            case KT_KEY_F5:
                return ImGuiKey_F5;
            case KT_KEY_F6:
                return ImGuiKey_F6;
            case KT_KEY_F7:
                return ImGuiKey_F7;
            case KT_KEY_F8:
                return ImGuiKey_F8;
            case KT_KEY_F9:
                return ImGuiKey_F9;
            case KT_KEY_F10:
                return ImGuiKey_F10;
            case KT_KEY_F11:
                return ImGuiKey_F11;
            case KT_KEY_F12:
                return ImGuiKey_F12;
            default:
                return ImGuiKey_None;
        }
    }

    auto ImGuiLayer::updateModifierState() const -> void {
        Application& app{ Application::get() };
        ImGuiIO& io{ ImGui::GetIO() };
        GLFWwindow* window{ nullptr };

        try {
            // We expect the native window for Linux Window to be a GLFWwindow*
            window = std::any_cast<GLFWwindow*>(app.getWindow().getNativeWindow());

            io.AddKeyEvent(ImGuiMod_Ctrl,  InputManager::isKeyPressed(KT_KEY_LEFT_CONTROL) || InputManager::isKeyPressed(KT_KEY_RIGHT_CONTROL));
            io.AddKeyEvent(ImGuiMod_Shift,  InputManager::isKeyPressed(GLFW_KEY_LEFT_SHIFT) || InputManager::isKeyPressed(GLFW_KEY_RIGHT_SHIFT));
            io.AddKeyEvent(ImGuiMod_Alt,  InputManager::isKeyPressed(GLFW_KEY_LEFT_ALT) || InputManager::isKeyPressed(GLFW_KEY_RIGHT_ALT));
            io.AddKeyEvent(ImGuiMod_Super,  InputManager::isKeyPressed(GLFW_KEY_LEFT_SUPER) || InputManager::isKeyPressed(GLFW_KEY_RIGHT_SUPER));
        }
        catch (const std::bad_any_cast& exception) {
            KATE_APP_LOGGER_ERROR("Exception thrown std::any_cast. What: {}", exception.what());
        }
    }

    auto ImGuiLayer::onKeyPressedEvent(KeyPressedEvent& event) -> bool {
        ImGuiIO& io{ ImGui::GetIO() };

        updateModifierState();

        ImGuiKey imguiKey = ImGuiToKtKey(event.getKeyCode());
        io.AddKeyEvent(imguiKey, true);

        // In cse we want to propagate this event
        // we say it is not handled
        return false;
    }

    auto ImGuiLayer::onKeyReleasedEvent(KeyReleasedEvent& event) -> bool {
        ImGuiIO& io{ ImGui::GetIO() };

        ImGuiKey imguiKey = ImGuiToKtKey(event.getKeyCode());
        io.AddKeyEvent(imguiKey, false);

        //io.KeysDown[ImGuiToGlfwKey(event.getKeyCode())] = false;

        // In cse we want to propagate this event
        // we say it is not handled
        return false;
    }

    auto ImGuiLayer::onWindowResizedEvent(WindowResizedEvent& event) -> bool {
        ImGuiIO& io{ ImGui::GetIO() };
        io.DisplaySize = ImVec2{ static_cast<float>(event.getWidth()), static_cast<float>(event.getHeight()) };
        io.DisplayFramebufferScale = ImVec2{ 1.0f, 1.0f };

        // TODO: should not be done here
        glViewport(0, 0, event.getWidth(), event.getHeight());

        // In cse we want to propagate this event
        // we say it is not handled
        return false;
    }

    auto ImGuiLayer::onKeyTypedEvent(KeyCharEvent& event) -> bool {
        ImGuiIO& io{ ImGui::GetIO() };
        if (event.getChar() > 0 && event.getChar() < 0x10000)
            io.AddInputCharacter(event.getChar());

        return false;
    }

    ImGuiLayer::~ImGuiLayer() = default;
}
