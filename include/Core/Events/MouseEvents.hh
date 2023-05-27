//
// Created by kate on 5/25/23.
//

#ifndef KATE_ENGINE_MOUSE_EVENTS_HH
#define KATE_ENGINE_MOUSE_EVENTS_HH

// C++ Standard Library
#include <string_view>
#include <array>

// Third-Party Libraries
#include <fmt/format.h>

// Project headers
#include "Event.hh"


namespace kT {
    class MouseMovedEvent: public Event {
    public:
        MouseMovedEvent(double x, double y)
            :   Event{ EventType::MOUSE_MOVED_EVENT, static_cast<EventCategory>(INPUT_EVENT_CATEGORY | MOUSE_EVENT_CATEGORY) }
            ,   m_PositionX{ x }
            ,   m_PositionY{ y }
        {}

        [[nodiscard]]
        auto getPositionX() -> double { return m_PositionX; }

        [[nodiscard]]
        auto getPositionY() -> double { return m_PositionY; }

        [[nodiscard]]
        auto getType() const -> EventType override { return getStaticType(); }

        /**
         * Useful if we need to query the type of this event.
         * See <code>kT::EventDispatcher</code> for usage
         * */
        static auto getStaticType() -> EventType { return EventType::MOUSE_MOVED_EVENT; }

        auto displayData() const -> std::string override {
            return fmt::format("{} POS [{}, {}]", getFormattedStr(getType()).data(), m_PositionX, m_PositionY);
        }
    protected:
        auto toString() const -> std::string_view override { return getFormattedStr(getType()); }

        double m_PositionX{};
        double m_PositionY{};
    };

    class MouseEvent : public Event {
    public:
        /**
         * Useful if we need to query the type of this event.
         * See <code>kT::EventDispatcher</code> for usage
         * */
    protected:
        explicit MouseEvent(EventType type, EventCategory categories = EMPTY_EVENT_CATEGORY)
            :   Event{ type, static_cast<EventCategory>(MOUSE_EVENT_CATEGORY | INPUT_EVENT_CATEGORY | categories) } {}
    };

    class MouseButtonPressedEvent : public MouseEvent {
    public:
        MouseButtonPressedEvent(std::int32_t button)
                :   MouseEvent{ EventType::MOUSE_BUTTON_PRESSED_EVENT, MOUSE_BUTTON_EVENT_CATEGORY }
                ,   m_Button{ button }
        {}

        [[nodiscard]]
        auto getMouseButton() -> std::int32_t { return m_Button; }

        [[nodiscard]]
        auto getType() const -> EventType override { return getStaticType(); }

        /**
         * Useful if we need to query the type of this event.
         * See <code>kT::EventDispatcher</code> for usage
         * */
        static auto getStaticType() -> EventType { return EventType::MOUSE_BUTTON_PRESSED_EVENT; }

        auto displayData() const -> std::string override {
            constexpr static std::array<std::string_view, 3> NAME{ "LEFT_CLICK", "RIGHT_CLICK", "SCROLL_WHEEL_CLICK" };
            // we are just testing with a mouse with three buttons for now
            return fmt::format("{} BUTTON {}", getFormattedStr(getType()).data(), NAME[m_Button]);
        }
    protected:
        auto toString() const -> std::string_view override { return getFormattedStr(getType()); }

        std::int32_t m_Button{};
    };

    class MouseButtonReleasedEvent : public MouseEvent {
    public:
        MouseButtonReleasedEvent(std::int32_t button)
                :   MouseEvent{ EventType::MOUSE_BUTTON_RELEASED_EVENT, MOUSE_BUTTON_EVENT_CATEGORY }
                ,   m_Button{ button }
        {}

        [[nodiscard]]
        auto getMouseButton() -> std::int32_t { return m_Button; }

        [[nodiscard]]
        auto getType() const -> EventType override { return getStaticType(); }

        /**
         * Useful if we need to query the type of this event.
         * See <code>kT::EventDispatcher</code> for usage
         * */
        static auto getStaticType() -> EventType { return EventType::MOUSE_BUTTON_RELEASED_EVENT; }

        auto displayData() const -> std::string override {
            constexpr static std::array<std::string_view, 3> NAME{ "LEFT_CLICK", "RIGHT_CLICK", "SCROLL_WHEEL_CLICK" };
            // we are just testing with a mouse with three buttons for now
            return fmt::format("{} BUTTON {}", getFormattedStr(getType()).data(), NAME[m_Button]);
        }
    protected:
        auto toString() const -> std::string_view override { return getFormattedStr(getType()); }

        std::int32_t m_Button{};
    };

    class MouseScrollEvent : public MouseEvent {
    public:
        MouseScrollEvent(double xOffset, double yOffset)
                :   MouseEvent{ EventType::MOUSE_SCROLLED_EVENT }, m_OffsetX{ xOffset }, m_OffsetY{ yOffset }
        {}

        [[nodiscard]]
        auto getOffsetX() -> double { return m_OffsetX; }

        [[nodiscard]]
        auto getOffsetY() -> double { return m_OffsetY; }

        auto getType() const -> EventType override { return getStaticType(); }

        /**
         * Useful if we need to query the type of this event.
         * See <code>kT::EventDispatcher</code> for usage
         * */
        static auto getStaticType() -> EventType { return EventType::MOUSE_SCROLLED_EVENT; }

        auto displayData() const -> std::string override {
            return fmt::format("{} OFFSETS [{}, {}]", getFormattedStr(getType()).data(), m_OffsetX, m_OffsetY);
        }
    protected:
        auto toString() const -> std::string_view override { return getFormattedStr(getType()); }

        double m_OffsetX{};
        double m_OffsetY{};
    };
}

#endif // KATE_ENGINE_MOUSE_EVENTS_HH
