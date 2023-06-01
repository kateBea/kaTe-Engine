#include <functional>
#include <memory>
#include <cmath>

// Third-party libraries
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Project headers
#include <Core/Events/AppEvents.hh>
#include <Core/Logger.hh>

#include <kaTe/Application.hh>
#include <kaTe/Common.hh>

#include <Core/Layers/ImGuiLayer.hh>
#include <Platform/Window/LinuxInputManager.hh>

namespace kaTe {
    auto Application::init() -> void {
        KATE_APP_LOGGER_INFO("Initializing kaTe Engine");
        // Allocate and Initialize members
        initWindow();
        initLayerStack();
        initInputManager();
        initImGuiLayer();

        pushLayer(m_ImGuiLayer);

        // Display OpenGL versions being used
        KT_DISPLAY_OPENGL_TARGET_VERSION();
        KT_DISPLAY_OPENGL_VENDOR_VERSION();
        KATE_APP_LOGGER_DEBUG("Finished kaTe Engine initialization");
    }

    auto Application::loop() -> void {
        KATE_CORE_LOGGER_INFO("Entering main loop kaTe Engine");

        while (m_State == State::RUNNING) {
            SWAP_BG_COLOR_INTERVAL();
            for (auto& layer : *m_LayerStack)
                layer->onUpdate();

            m_ImGuiLayer->beginFrame();
            for (auto& layer : *m_LayerStack)
                // CAUTION. JUST FOR TESTING PURPOSES FOR NOW
                // UB if the layer is not an ImGuiLayer
                reinterpret_cast<ImGuiLayer*>(&layer)->imGuiPushRenderElements();
            m_ImGuiLayer->endFrame();


            m_Window->onUpdate();
        }
    }

    auto Application::initWindow() -> void {
        KATE_APP_LOGGER_INFO("Initializing Application::Window kaTe Engine");
        m_Window = std::make_unique<LinuxWindow>();
        KT_ASSERT(m_Window != nullptr, "Window is NULL");

        m_Window->init();
        m_Window->setEventCallback(KT_BIND_EVENT_FUNC(Application::onEvent));
    }

    auto Application::initLayerStack() -> void {
        KATE_APP_LOGGER_INFO("Initializing Application::LayerStack kaTe Engine");
        m_LayerStack = std::make_unique<LayerStack>();
        KT_ASSERT(m_LayerStack != nullptr, "Layer Stack is NULL");

        m_LayerStack->init();
    }

    auto Application::initInputManager() -> void {
        KATE_APP_LOGGER_INFO("Initializing Application::InputManager kaTe Engine");
        m_InputManager = std::make_unique<LinuxInputManager>();
        KT_ASSERT(m_InputManager != nullptr, "Input Manager is NULL");
    }


    auto Application::onEvent(Event& event) -> void {
        KATE_APP_LOGGER_TRACE("{}", event.displayData());

        EventDispatcher evDis{ event };
        if (evDis.forward<WindowCloseEvent>(KT_BIND_EVENT_FUNC(Application::onWindowClose)))
            KATE_APP_LOGGER_TRACE("HANDLED {}", event.displayData());

        for (auto it{ m_LayerStack->rbegin() }; it != m_LayerStack->rend(); ++it) {
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
        m_LayerStack->addLayer(layer);
        layer->onAttach();
    }

    auto Application::pushOverlay(LayerStack::LayerPtr overlay) -> void {
        m_LayerStack->addOverlay(overlay);
        overlay->onAttach();
    }

    auto Application::shutDown() -> void {
        KATE_APP_LOGGER_INFO("Shutting down kaTe Engine");

        m_LayerStack->shutDown();
        m_Window->shutDown();
    }

    auto Application::getWindow() -> Window & {
        KT_ASSERT(m_Window, "Application main window is NULL");
        return *m_Window;
    }

    auto Application::getInputManager() -> InputManager& {
        KT_ASSERT(m_InputManager, "Input manager is null");
        return *m_InputManager;
    }
    auto Application::initImGuiLayer() -> void {
        KATE_APP_LOGGER_INFO("Initializing Application::ImGuiLayer kaTe Engine");
        m_ImGuiLayer = std::make_shared<ImGuiLayer>();
        KT_ASSERT(m_InputManager != nullptr, "ImGui Layer is NULL");
    }
}