/**
 * AppEvents.hh
 * Created by kate on 5/26/23.
 * */

#ifndef KATE_ENGINE_APP_EVENTS_HH
#define KATE_ENGINE_APP_EVENTS_HH

// C++ Standard Library
#include <string_view>

// Third-Party Libraries
#include <fmt/core.h>

// Project Headers
#include <Core/Events/Event.hh>
#include <kaTe/Common.hh>

namespace kT {
    class WindowResizedEvent : public Event {
    public:
        WindowResizedEvent(Int32_T newWidth, Int32_T newHeight)
            :   Event{ EventType::WINDOW_RESIZE_EVENT, APPLICATION_EVENT_CATEGORY }, m_Width{ newWidth }, m_Height{ newHeight } {}

        [[nodiscard]]
        auto getWidth() const -> Int32_T { return m_Width; }

        [[nodiscard]]
        auto getHeight() const -> Int32_T { return m_Height; }

        auto getType() const -> EventType override { return getStaticType(); }

        /**
         * Useful if we need to query the type of this event.
         * See <code>kT::EventDispatcher</code> for usage
         * */
        static auto getStaticType() -> EventType { return EventType::WINDOW_RESIZE_EVENT; }

        auto displayData() const -> std::string override {
            return fmt::format("{} NEW DIMENSIONS: [{}, {}]", getFormattedStr(getType()).data(), m_Width, m_Height);
        }

    protected:
        auto toString() const -> std::string_view override { return getFormattedStr(getType()); }

        Int32_T m_Width{};
        Int32_T m_Height{};
    };

    class WindowCloseEvent: public Event {
    public:
        WindowCloseEvent() : Event{ EventType::WINDOW_CLOSE_EVENT, APPLICATION_EVENT_CATEGORY } {}

        auto getType() const -> EventType override { return getStaticType(); }

        /**
         * Useful if we need to query the type of this event.
         * See <code>kT::EventDispatcher</code> for usage
         * */
        static auto getStaticType() -> EventType { return EventType::WINDOW_CLOSE_EVENT; }

        auto displayData() const -> std::string override {
            return fmt::format("{}", getFormattedStr(getType()).data());
        }
    protected:
        auto toString() const -> std::string_view override { return getFormattedStr(getType()); }
    };

    class AppTick : public Event {
    public:
        AppTick() : Event{ EventType::APP_TICK_EVENT, APPLICATION_EVENT_CATEGORY } {}

        auto getType() const -> EventType override { return getStaticType(); }

        /**
         * Useful if we need to query the type of this event.
         * See <code>kT::EventDispatcher</code> for usage
         * */
        static auto getStaticType() -> EventType { return EventType::APP_TICK_EVENT; }

        auto displayData() const -> std::string override {
            return fmt::format("{}", getFormattedStr(getType()).data());
        }

    protected:
        auto toString() const -> std::string_view override { return getFormattedStr(getType()); }
    };

    class AppUpdate : public Event {
    public:
        AppUpdate() : Event{ EventType::APP_UPDATE_EVENT, APPLICATION_EVENT_CATEGORY } {}

        auto getType() const -> EventType override { return getStaticType(); }

        /**
         * Useful if we need to query the type of this event.
         * See <code>kT::EventDispatcher</code> for usage
         * */
        static auto getStaticType() -> EventType { return EventType::APP_UPDATE_EVENT; }

        auto displayData() const -> std::string override {
            return fmt::format("{}", getFormattedStr(getType()).data());
        }
    protected:
        auto toString() const -> std::string_view override { return getFormattedStr(getType()); }
    };

    class AppRender: public Event {
    public:
        AppRender() : Event{ EventType::APP_RENDER_EVENT, APPLICATION_EVENT_CATEGORY } {}

        auto getType() const -> EventType override { return getStaticType(); }

        /**
         * Useful if we need to query the type of this event.
         * See <code>kT::EventDispatcher</code> for usage
         * */
        static auto getStaticType() -> EventType { return EventType::APP_RENDER_EVENT; }

        auto displayData() const -> std::string override {
            return fmt::format("{}", getFormattedStr(getType()).data());
        }
    protected:
        auto toString() const -> std::string_view override { return getFormattedStr(getType()); }
    };


}   // END NAMESPACE kT

#endif // KATE_ENGINE_APP_EVENTS_HH
