#include <string>
#include <utility>
#include <functional>
#include <memory>
#include <cmath>

// Third-party libraries
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Project headers
#include "Core/Events/AppEvents.hh"
#include "Core/Events/KeyEvents.hh"
#include "Core/Events/MouseEvents.hh"
#include "Core/Logger.hh"

#include "../include/Common.hh"
#include "../include/Application.hh"

// just useful for the Application class for now
#define BIND_EVENT_FUNC(x) \
    std::bind(&Application::x, this, std::placeholders::_1)

namespace kT {
    Application::Application()
        :   m_Window{}, m_State{ State::RUNNING }
    {}

    auto Application::init() -> void {
#if defined(KT_ENABLE_LOGGING)
        kT::Logger::init();
#endif
        KATE_APP_LOGGER_INFO("Initializing kaTe Engine");
        KATE_APP_LOGGER_INFO("Initializing new window kaTe Engine");

        m_Window = LinuxWindow::spawn();
        m_Window->setEventCallback(BIND_EVENT_FUNC(onEvent));

        // Display OpenGL versions being used
        KT_DISPLAY_OPENGL_TARGET_VERSION();
        KT_DISPLAY_OPENGL_VENDOR_VERSION();
    }

    auto Application::loop() -> void {
        KATE_CORE_LOGGER_INFO("Entering main loop kaTe Engine");

        while (m_State == State::RUNNING) {
            SWAP_BG_COLOR_INTERVAL();
            m_Window->onUpdate();
            for (auto& layer : m_LayerStack)
                layer->onUpdate();
        }
    }

    auto Application::shutdown() -> void {
        KATE_APP_LOGGER_INFO("Shutting down kaTe Engine");
    }

    auto Application::onEvent(Event& event) -> void {
        KATE_APP_LOGGER_TRACE("{}", event.displayData());

        EventDispatcher evDis{ event };
        if (evDis.dispatch<WindowCloseEvent>(BIND_EVENT_FUNC(onWindowClose)))
            KATE_APP_LOGGER_TRACE("HANDLED {}", event.displayData());

        for (auto it{ m_LayerStack.rbegin() }; it != m_LayerStack.rend(); ++it) {
            (*it)->onEvent(event);
            if (event.isHandled())
                break;
        }
    }

    bool Application::onWindowClose(WindowCloseEvent& ev) {
        m_State = State::STOPPED;
        return true;
    }

    auto Application::pushLayer(LayerStack::LayerPtr layer) -> void {
        m_LayerStack.addLayer(layer);
    }

    auto Application::pushOverlay(LayerStack::LayerPtr overlay) -> void {
        m_LayerStack.addOverlay(overlay);
    }
}