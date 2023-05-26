#include <iostream>
#include <string>
#include <utility>

#include "Core/Events/Event.hh"
#include "Core/Events/KeyEvents.hh"
#include "Core/Events/MouseEvents.hh"

#include "Core/Assert.hh"
#include "Core/Logger.hh"

#include "../include/Core.hh"
#include "../include/Application.hh"

namespace kT {
    Application::Application()
        :   m_Window{}, m_State{ State::RUNNING }
    {}

    auto Application::init() -> void {
        kT::Logger::init();
        KATE_LOGGER_INFO("Initializing kaTe Engine");

        KATE_LOGGER_INFO("Initializing new window kaTe Engine");
        m_Window = LinuxWindow::spawn();

    }

    auto Application::loop() -> void {
        KATE_LOGGER_INFO("Entering main loop kaTe Engine");
        while (m_State == State::RUNNING) {
            // Poll Events
            m_Window->onUpdate();

            // Update state of engine
            // Render
        }
    }

    auto Application::shutdown() -> void {
        KATE_LOGGER_INFO("Shutting down kaTe Engine");
    }
}