/**
 * KeyEvents.hh
 * Created by kate on 5/25/23.
 * */

#ifndef KATE_ENGINE_KEY_EVENTS_HH
#define KATE_ENGINE_KEY_EVENTS_HH

// C++ Standard Library
#include <cstdint>
#include <string_view>
#include <iomanip>
#include <cstdio>

// Third-Party Libraries
#include <fmt/format.h>

// Project Headers
#include <Core/Events/Event.hh>
#include <kaTe/Common.hh>

namespace kT {
    /**
     * Common interface for Key Events
     * */
    class KeyEvent : public Event {
    public:
        auto getKeyCode() const -> Int32_T { return m_KeyCode; }

    protected:
        /**
         * Only specializations may create events
         * */
        KeyEvent(EventType type, Int32_T keyCode)
            :   Event{ type, static_cast<EventCategory>(INPUT_EVENT_CATEGORY | KEY_EVENT_CATEGORY) }, m_KeyCode{ keyCode }
        {}

        std::int32_t m_KeyCode{};
    };

    class KeyPressedEvent : public KeyEvent {
    public:
        KeyPressedEvent(Int32_T keyCode, bool repeated)
            :   KeyEvent{ EventType::KEY_PRESSED_EVENT, keyCode }, m_Repeated{ repeated }
        {}

        [[nodiscard]]
        auto isRepeated() const -> bool { return m_Repeated; }

        [[nodiscard]]
        auto getType() const -> EventType override { return getStaticType(); }

        /**
         * Useful if we need to query the type of this event.
         * See <code>kT::EventDispatcher</code> for usage
         * */
        static auto getStaticType() -> EventType { return EventType::KEY_PRESSED_EVENT; }

        auto displayData() const -> std::string override {
            return fmt::format("{} KEY: {} REPEATED: {}", getFormattedStr(getType()).data(), m_KeyCode, m_Repeated ? "TRUE" : "FALSE");
        }

    private:
        auto toString() const -> std::string_view override { return getFormattedStr(getType()); }
        bool m_Repeated{};
    };

    class KeyReleasedEvent : public KeyEvent {
    public:
        KeyReleasedEvent(Int32_T code)
            :   KeyEvent{ EventType::KEY_RELEASED_EVENT, code }
        {}

        [[nodiscard]]
        auto getType() const -> EventType override { return getStaticType(); }

        /**
         * Useful if we need to query the type of this event.
         * See <code>kT::EventDispatcher</code> for usage
         * */
        static auto getStaticType() -> EventType { return EventType::KEY_RELEASED_EVENT; }

        auto displayData() const -> std::string override {
            return fmt::format("{} KEY: {}", getFormattedStr(getType()).data(), m_KeyCode);
        }
    private:
        auto toString() const -> std::string_view override { return getFormattedStr(getType()); }
    };

    class KeyCharEvent : public Event {
    public:
        KeyCharEvent(UInt32_T charCode)
            :   Event{ EventType::KEY_CHAR_EVENT, static_cast<EventCategory>(INPUT_EVENT_CATEGORY | KEY_EVENT_CATEGORY) }
            ,   m_KeyChar{ charCode }
        {}

        [[nodiscard]]
        auto getType() const -> EventType override { return getStaticType(); }

        [[nodiscard]]
        auto getChar() -> UInt32_T { return m_KeyChar; }

        /**
         * Useful if we need to query the type of this event.
         * See <code>kT::EventDispatcher</code> for usage
         * */
        static auto getStaticType() -> EventType { return EventType::KEY_CHAR_EVENT; }

        auto displayData() const -> std::string override {
            return fmt::format("{} KEY: {}", getFormattedStr(getType()).data(), m_KeyChar);
        }
    private:
        auto toString() const -> std::string_view override { return getFormattedStr(getType()); }

        UInt32_T m_KeyChar{};
    };

}   // END NAMESPACE kT

#endif //KATE_ENGINE_KEY_EVENTS_HH
