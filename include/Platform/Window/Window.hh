/**
 * Window.hh
 * Created by kate on 5/26/23.
 * */
#ifndef KATE_ENGINE_WINDOW_HH
#define KATE_ENGINE_WINDOW_HH

// C++ Standard Library
#include <string>
#include <string_view>
#include <cstdint>
#include <functional>
#include <memory>
#include <utility>
#include <any>

// Project Headers
#include <Core/Events/Event.hh>
#include <kaTe/Common.hh>

namespace kT {
    class WindowProperties {
    public:
        explicit WindowProperties(std::string_view name = "kaTe Engine", std::int32_t width = 1280, std::int32_t height = 720)
            :   m_Title{ name }, m_Width{ width }, m_Height{ height }
        {}

        [[nodiscard]]
        auto getName() const -> const std::string& { return m_Title; }

        [[nodiscard]]
        auto getWidth() const -> Int32_T { return m_Width; }

        [[nodiscard]]
        auto getHeight() const -> Int32_T { return m_Height; }

        auto setWidth(Int32_T width) -> void { m_Width = width; }
        auto setHeight(Int32_T height) -> void { m_Height = height; }

    private:
        std::string m_Title{};
        Int32_T  m_Width{};
        Int32_T  m_Height{};
    };

    /**
    * General interface for desktop Windows. We may define different types of
    * windows depending on the platform if extra platform support is necessary
     * to create a context for a specific graphics API.
     *
     * Important to initialize and terminate the windows explicitly for now by explicit calls
     * to <code>init()</code> and <code>shutDown()</code>, this allows for more flexibility as to
     * when an entity has to free its resources amongst other termination operations
     *
     * A single instance of Window manages a single window, hence why the copy
     * operations are disabled and move semantics are enabled, this way if we attempt
     * to copy a Window we will get a compile error to ensure this constrain, what we can do instead
     * is moving ownership of a Window resources from one to another by using move semantics
    * */
    class Window {
    public:
        using EventCallbackFunc_T = std::function<void(Event&)>;

        explicit Window()
            :   m_WindowCreateSuccess{ false } {}

        Window(Window&& other) noexcept
            :   m_Properties{ std::move(other.m_Properties) } {}

        virtual auto operator=(Window&& other) noexcept -> Window& {
            m_Properties = std::move(other.m_Properties);
            return *this;
        }

        [[nodiscard]]
        virtual auto getWidth() const -> Int32_T = 0;
        [[nodiscard]]
        virtual auto getHeight() const -> Int32_T = 0;

        /**
         * Returns a pointer to a structure containing the
         * native Window structure
         * */
        [[nodiscard]]
        virtual auto getNativeWindow() -> std::any = 0;

        virtual auto init() -> void = 0;
        virtual auto onUpdate() -> void = 0;
        virtual auto shutDown() -> void = 0;
        virtual auto setEventCallback(EventCallbackFunc_T func) -> void = 0;

        [[nodiscard]]
        virtual auto isVSyncEnabled() const -> bool = 0;
        virtual auto enableVSync() -> void = 0;
        virtual auto disableVSync() -> void = 0;

        virtual ~Window() = default;
    protected:
        bool m_WindowCreateSuccess{};

    private:
        WindowProperties m_Properties{};
    };

}   // END NAMESPACE kT

#endif // KATE_ENGINE_WINDOW_HH
