//
// Created by kate on 5/26/23.
//

#ifndef KATE_ENGINE_WINDOW_HH
#define KATE_ENGINE_WINDOW_HH

#include <string>
#include <string_view>
#include <cstdint>
#include <functional>
#include <memory>
#include <utility>

#include "../../Core/Events/Event.hh"

namespace kT {
    class WindowProperties {
    public:
        WindowProperties(std::string_view name = "kaTe Engine", std::int32_t width = 1920, std::int32_t height = 1080)
            :   m_Title{ name }, m_Width{ width }, m_Height{ height }
        {}

        [[nodiscard]]
        auto getName() const -> const std::string& { return m_Title; }

        [[nodiscard]]
        auto getWidth() const -> std::int32_t { return m_Width; }

        [[nodiscard]]
        auto getHeight() const -> std::int32_t { return m_Height; }

    private:
        std::string m_Title{};
        std::int32_t  m_Width{};
        std::int32_t m_Height{};
    };

    /**
    * General interface for desktop Windows. We may define different types of
    * windows depending on the platform if extra platform support is necessary
     * to create a context for a specific graphics API
    * */
    class Window {
    public:
        using EventCallbackFunc = std::function<void(Event&)>;

        explicit Window(const WindowProperties& properties = WindowProperties{})
            :   m_Properties{ properties }
        {}

        Window(Window&& other)
            :   m_Properties{ std::move(other.m_Properties) }
        {}

        auto operator=(Window&& other) -> Window& {
            m_Properties = std::move(other.m_Properties);
            return *this;
        }

        virtual auto onUpdate() -> void = 0;

        [[nodiscard]]
        virtual auto getWidth() const -> std::int32_t = 0;
        [[nodiscard]]
        virtual auto getHeight() const -> std::int32_t = 0;

        virtual auto setEventCallback(EventCallbackFunc func) -> void = 0;

        virtual auto enableVSync() -> void = 0;
        virtual auto disableVSync() -> void = 0;

        [[nodiscard]]
        virtual auto isVSyncEnabled() const -> bool = 0;

        virtual ~Window() = default;
    protected:
        virtual auto startUp() -> void = 0;
        virtual auto shutDown() -> void = 0;
    private:
        WindowProperties m_Properties{};
    };
}

#endif //KATE_ENGINE_WINDOW_HH
