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
#include <Tools/Common.hh>
#include <Renderer/RenderContext.hh>

namespace kaTe {
    /**
     * Tells whether the GLFW was initialized successfully.
     * Needed before creating windows
     * */
    inline bool g_GLFWInitSuccess{ false };

    /**
     * Window specialization for Linux.
     * */
    class WindowGLFW : public Window {
    public:
        explicit WindowGLFW(const WindowProperties& properties = WindowProperties{});

        [[nodiscard]]
        auto getWidth() const -> Int32_T override { return m_Properties.getWidth(); }
        [[nodiscard]]
        auto getHeight() const -> Int32_T override { return m_Properties.getHeight(); }
        /**
         * Returns a pointer to a structure containing the
         * native Window structure
         * */
        [[nodiscard]]
        auto getNativeWindow() -> std::any override { return m_Window; }

        auto init() -> void override;
        auto onUpdate() -> void override;
        auto shutDown() -> void override;
        auto setEventCallback(EventCallbackFunc_T func) -> void override { m_Callback = func; }
        [[nodiscard]]
        auto isVSyncEnabled() const -> bool override { return m_VSync; }
        auto enableVSync() -> void override;
        auto disableVSync() -> void override;

        ~WindowGLFW() override = default;

    private:
        // Helpers
        auto setCallbacks() -> void;
        auto spawnOnCenter() const -> void;
        static auto getActiveAPIContext() -> RenderContext*;
        static auto initGLFW() -> void;

        RenderContext* m_Context{};
        GLFWwindow* m_Window{};
        EventCallbackFunc_T m_Callback{};
        bool m_VSync{};
    };

}   // END NAMESPACE kT

#endif // KATE_ENGINE_LINUX_WINDOW_HH
