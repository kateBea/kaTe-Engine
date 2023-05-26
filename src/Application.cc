#include <iostream>
#include <string>

#include "Core/Events/Event.hh"
#include "Core/Events/KeyEvents.hh"
#include "Core/Events/MouseEvents.hh"

#include "Core/Assert.hh"
#include "Core/Logger.hh"

#include "../include/Core.hh"
#include "../include/Application.hh"

namespace kT {
    auto Application::init() -> void {
#if defined(_DEBUG)
        kT::Logger::init();
        KATE_LOGGER_INFO("Initializing kaTe Engine");
#endif

        KeyPressedEvent ev{ 14, false };
        EventDispatcher disp{ ev };

        // just to make it compile
        EventDispatcher::EventFuncType<KeyPressedEvent> handler {
            [](Event& ev) -> bool { KATE_LOGGER_DEBUG("Event [{}] handled", ev.getNameStr()); }
        };

        if (disp.dispatch<KeyPressedEvent>(handler))
            KATE_LOGGER_DEBUG("Dispatching KeyPressedEvent");

    }

    auto Application::loop() -> void {
#if defined(_DEBUG)
        KATE_LOGGER_INFO("Entering main loop kaTe Engine");
#endif
        while (true) {
            static std::string str{};
            std::cout << "Enter something: ";
            std::cin >> str;

            if (str == "exit")
                break;


            // Poll Events
            // Update state of engine
            // Render
        }
    }

    auto Application::shutdown() -> void {
#if defined(_DEBUG)
        KATE_LOGGER_INFO("Shutting down kaTe Engine");
#endif
    }
}