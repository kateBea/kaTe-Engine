#include <iostream>
#include <string>
#include <utility>
#include <functional>

// Events
#include "Core/Events/Event.hh"
#include "Core/Events/AppEvents.hh"
#include "Core/Events/KeyEvents.hh"
#include "Core/Events/MouseEvents.hh"

#include "Core/Assert.hh"
#include "Core/Logger.hh"

#include "../include/Core.hh"
#include "../include/Application.hh"

// just useful for the Application class for now
#define BIND_EVENT_FUNC(x) std::bind(&Application::x, this, std::placeholders::_1)

namespace kT {
    Application::Application()
        :   m_Window{}, m_State{ State::RUNNING }
    {}

    auto Application::init() -> void {
#if defined(KT_ENABLE_LOGGING)
        kT::Logger::init();
#endif

        KATE_LOGGER_INFO("Initializing kaTe Engine");
        KATE_LOGGER_INFO("Initializing new window kaTe Engine");
        m_Window = LinuxWindow::spawn();
        m_Window->setEventCallback(BIND_EVENT_FUNC(onEvent));
    }

    auto Application::loop() -> void {
        KATE_LOGGER_INFO("Entering main loop kaTe Engine");
        while (m_State == State::RUNNING) {
            // Poll Events

            glClearColor(0.3f, 0.8f, 0.45f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            m_Window->onUpdate();

            // Update state of engine
            // Render
        }
    }

    auto Application::shutdown() -> void {
        KATE_LOGGER_INFO("Shutting down kaTe Engine");
    }

    auto Application::onEvent(Event& event) -> void {
        KATE_LOGGER_TRACE("{}", event.displayData());

        EventDispatcher evDis{ event };
        if (evDis.dispatch<WindowCloseEvent>(BIND_EVENT_FUNC(onWindowClose)))
            KATE_LOGGER_TRACE("HANDLED {}", event.displayData());
    }

    bool Application::onWindowClose(WindowCloseEvent& ev) {
        m_State = State::STOPPED;
        return true;
    }
}