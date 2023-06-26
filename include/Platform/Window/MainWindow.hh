/**
 * LinuxWindow.hh
 * Created by kate on 5/26/23.
 * */

#ifndef KATE_ENGINE_LINUX_WINDOW_HH
#define KATE_ENGINE_LINUX_WINDOW_HH

// C++ Standard Library
#include <any>

// Third-Party Libraries
#include <GLFW/glfw3.h>

// Project Headers
#include <Platform/Window/Window.hh>
#include <Renderer/RenderContext.hh>

#include <Tools/Common.hh>

namespace kaTe {
    /**
     * Tells whether the GLFW was initialized successfully.
     * Needed before creating windows
     * */
    inline bool g_GLFWInitSuccess{ false };

    /**
     * Window specialization for Linux.
     * */
    class MainWindow : public Window {
    public:
        explicit MainWindow(const WindowProperties& properties = WindowProperties{});

        KT_NODISCARD auto GetWidth() const -> Int32_T override { return m_Properties.GetWidth(); }
        KT_NODISCARD auto GetHeight() const -> Int32_T override { return m_Properties.GetHeight(); }
        /**
         * Returns a pointer to a structure containing the
         * native Window structure
         * */
        KT_NODISCARD auto GetNativeWindow() -> std::any override { return m_Window; }

        auto Init() -> void override;
        auto OnUpdate() -> void override;
        auto ShutDown() -> void override;
        auto SetEventCallback(EventCallbackFunc_T func) -> void override { m_Callback = func; }

        KT_NODISCARD auto IsVSyncEnabled() const -> bool override { return m_VSync; }
        auto EnableVSync() -> void override;
        auto DisableVSync() -> void override;

        ~MainWindow() override = default;

    private:
        // Helpers
        auto SetCallbacks() -> void;
        auto SpawnOnCenter() const -> void;
        static auto GetActiveAPIContext() -> RenderContext*;
        static auto InitGLFW() -> void;

        RenderContext*      m_Context{};
        GLFWwindow*         m_Window{};
        EventCallbackFunc_T m_Callback{};
        bool                m_VSync{};
    };

}   // END NAMESPACE kT

#endif // KATE_ENGINE_LINUX_WINDOW_HH
