//
// Created by kate on 5/26/23.
//

#ifndef KATE_ENGINE_LINUX_WINDOW_HH
#define KATE_ENGINE_LINUX_WINDOW_HH

// C++ Standard Library
#include <memory>
#include <utility>

// Third-Party Library
#include <GLFW/glfw3.h>

// Project
#include "Window.hh"

namespace kT {
    /**
     * Tells whether the GLFW was initialized successfully.
     * Needed before creating windows
     * */
    static bool s_GLFWInitSuccess{ false };

    /**
     * Window specialization for Linux.
     * */
    class LinuxWindow : public Window {
    public:
        using EventCallbackFunc = std::function<void(Event&)>;

        explicit LinuxWindow(const WindowProperties& properties = WindowProperties{})
            :   m_Data{ .properties{ properties }, .callback{}, .VSync{ true } }, m_Window{ nullptr }
        {
            startUp();
        }

        LinuxWindow(LinuxWindow&& other)
            :   Window{ std::move(other) }, m_Data{ std::move(other.m_Data) }, m_Window{ other.m_Window }
        {
            other.m_Window = nullptr;
            startUp();
        }

        auto operator=(LinuxWindow&& other) noexcept -> LinuxWindow& {
            m_Data = std::move(other.m_Data);
            m_Window = std::move(other.m_Window);

            other.m_Window = nullptr;

            return *this;
        }

        auto onUpdate() -> void override;

        [[nodiscard]]
        auto getWidth() const -> std::int32_t override { return m_Data.properties.getWidth(); }
        [[nodiscard]]
        auto getHeight() const -> std::int32_t override { return m_Data.properties.getHeight(); }

        auto setEventCallback(EventCallbackFunc func) -> void override { m_Data.callback = func; }

        auto enableVSync() -> void override;
        auto disableVSync() -> void override;

        [[nodiscard]]
        auto isVSyncEnabled() const -> bool override { return m_Data.VSync; }

        ~LinuxWindow() override;

        [[nodiscard]]
        static auto spawn() -> std::unique_ptr<LinuxWindow>;
    private:
        auto startUp() -> void override;
        auto shutDown() -> void override;

        struct WindowData {
            WindowProperties properties{};
            EventCallbackFunc callback{};
            bool VSync{};
        };

        GLFWwindow* m_Window{};
        WindowData m_Data;
    };
}

#endif // KATE_ENGINE_LINUX_WINDOW_HH
