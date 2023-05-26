//
// Created by kate on 5/25/23.
//

#ifndef KATE_ENGINE_KEY_EVENTS_HH
#define KATE_ENGINE_KEY_EVENTS_HH

#include <cstdint>
#include <string_view>

#include "Event.hh"

namespace kT {
    /**
     * Common interface for Key Events
     * */
    class KeyEvent : public Event {
    public:
        auto getKeyCode() const -> std::int32_t { return m_KeyCode; }

    protected:
        /**
         * Only specializations may create events
         * */
        KeyEvent(EventType type, std::int32_t code)
            :   Event{ type, static_cast<EventCategory>(INPUT_EVENT_CATEGORY | KEY_EVENT_CATEGORY) }, m_KeyCode{ code }
        {}

        std::int32_t m_KeyCode{};
    };

    class KeyPressedEvent : public KeyEvent {
    public:
        KeyPressedEvent(std::int32_t code, bool repeated)
            :   KeyEvent{ EventType::KEY_PRESSED_EVENT, code }, m_Repeated{ repeated }
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
    private:
        auto toString() const -> std::string_view override { return getFormattedStr(getType()); }
        bool m_Repeated{};
    };

    class KeyReleasedEvent : public KeyEvent {
    public:
        KeyReleasedEvent(std::int32_t code)
            :   KeyEvent{ EventType::KEY_RELEASED_EVENT, code }
        {}

        [[nodiscard]]
        auto getType() const -> EventType override { return getStaticType(); }

        /**
         * Useful if we need to query the type of this event.
         * See <code>kT::EventDispatcher</code> for usage
         * */
        static auto getStaticType() -> EventType { return EventType::KEY_RELEASED_EVENT; }
    private:
        auto toString() const -> std::string_view override { return getFormattedStr(getType()); }
    };
}

#endif //KATE_ENGINE_KEY_EVENTS_HH
