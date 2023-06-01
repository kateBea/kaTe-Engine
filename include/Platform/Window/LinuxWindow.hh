/**
 * LinuxWindow.hh
 * Created by kate on 5/26/23.
 * */

#ifndef KATE_ENGINE_LINUX_WINDOW_HH
#define KATE_ENGINE_LINUX_WINDOW_HH

// C++ Standard Library
#include <memory>
#include <utility>

// Third-Party Libraries
#include <GLFW/glfw3.h>

// Project Headers
#include <Platform/Window/Window.hh>
#include <kaTe/Common.hh>

namespace kaTe {
    /**
     * Tells whether the GLFW was initialized successfully.
     * Needed before creating windows
     * */
    inline bool g_GLFWInitSuccess{ false };

    /**
     * Window specialization for Linux.
     * */
    class LinuxWindow : public Window {
    public:
        explicit LinuxWindow(const WindowProperties& properties = WindowProperties{});

        /**
         * Constructs and initializes this LinuxWindow using move semantics.
         * If this call is successful, <code>other</code> is put into an invalid
         * state thus accessing its data may result in undefined behaviour.
         * @param other moved from object
         * */
        LinuxWindow(LinuxWindow&& other);

        /**
         * Assigns <code>other</code> to this LinuxWindow using move semantics.
         * If this call is successful, <code>other</code> is put into an invalid
         * state thus accessing its data may result in undefined behaviour.
         * @param other moved from object
         * */
        auto operator=(LinuxWindow&& other) noexcept -> LinuxWindow&;

        [[nodiscard]]
        auto getWidth() const -> Int32_T override { return m_Data.properties.getWidth(); }
        [[nodiscard]]
        auto getHeight() const -> Int32_T override { return m_Data.properties.getHeight(); }
        /**
         * Returns a pointer to a structure containing the
         * native Window structure
         * */
        [[nodiscard]]
        auto getNativeWindow() -> std::any override { return m_Window; }

        auto init() -> void override;
        auto onUpdate() -> void override;
        auto shutDown() -> void override;
        auto setEventCallback(EventCallbackFunc_T func) -> void override { m_Data.callback = func; }
        [[nodiscard]]
        auto isVSyncEnabled() const -> bool override { return m_Data.VSync; }
        auto enableVSync() -> void override;
        auto disableVSync() -> void override;

        ~LinuxWindow() override = default;

    private:
        auto setCallbacks() -> void;

        /**
         * This struct exists so we can store a pointer to it via
         * glfwGetWindowUserPointer() and retrieve the data contained with in
         * inside the function callbacks
         * */
        struct WindowData {
            WindowProperties properties{};
            EventCallbackFunc_T callback{};
            bool VSync{};
        };

        GLFWwindow* m_Window{};
        WindowData m_Data;
    };

}   // END NAMESPACE kT

#endif // KATE_ENGINE_LINUX_WINDOW_HH
