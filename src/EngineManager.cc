#include <functional>
#include <memory>
#include <cmath>

// Third-party libraries
#include <GLFW/glfw3.h>

// Project headers
#include <Core/Events/AppEvents.hh>
#include <Core/Logger.hh>

#include "Core/EngineManager.hh"
#include <Tools/Common.hh>

#include <Core/Layers/ImGuiLayer.hh>
#include <Platform/InputManagerGLFW.hh>

#include <Renderer/Renderer.hh>


namespace kaTe {
    auto EngineManager::init() -> void {
        KATE_CORE_LOGGER_INFO("Initializing kaTe Engine ----------------------------------------");

        initWindow();
        initLayerStack();
        initInputManager();
        initRenderer();
        initOrthographicCamera();

        KATE_CORE_LOGGER_DEBUG("Finished kaTe Engine initialization ----------------------------");
    }

    auto EngineManager::initWindow() -> void {
        KATE_CORE_LOGGER_INFO("kaTe Engine: Main Window initialization");
        m_Window = std::make_unique<WindowGLFW>();
        KT_ASSERT(m_Window != nullptr, "Window is NULL");

        m_Window->init();

        // Should probably not be done here
        m_Window->setEventCallback(KT_BIND_EVENT_FUNC(EngineManager::onEvent));
    }

    auto EngineManager::initLayerStack() -> void {
        KATE_CORE_LOGGER_INFO("kaTe Engine: Layer Stack initialization");
        m_LayerStack = std::make_unique<LayerStack>();
        KT_ASSERT(m_LayerStack != nullptr, "Layer Stack is NULL");

        m_LayerStack->init();

        SPtr_T<ImGuiLayer> temp{ std::make_shared<ImGuiLayer>() };
        temp->onAttach();
        m_LayerStack->addLayer(temp);
    }

    auto EngineManager::initInputManager() -> void {
        KATE_CORE_LOGGER_INFO("kaTe Engine: Input Manager initialization");
        m_InputManager = std::make_unique<InputManagerGLFW>();
        KT_ASSERT(m_InputManager != nullptr, "Input Manager is NULL");
    }

    // Should probably not be here
    auto EngineManager::onEvent(Event& event) -> void {
        KATE_CORE_LOGGER_TRACE("{}", event.displayData());

        EventDispatcher evDis{ event };
        if (evDis.forward<WindowCloseEvent>(KT_BIND_EVENT_FUNC(EngineManager::onWindowClose)))
            KATE_CORE_LOGGER_TRACE("HANDLED {}", event.displayData());

        if (evDis.forward<WindowResizedEvent>(KT_BIND_EVENT_FUNC(EngineManager::onResizeEvent)))
            KATE_CORE_LOGGER_TRACE("HANDLED {}", event.displayData());

        for (auto it{ m_LayerStack->rbegin() }; it != m_LayerStack->rend(); ++it) {
            (*it)->onEvent(event);
            if (event.isHandled())
                break;
        }
    }

    bool EngineManager::onWindowClose(WindowCloseEvent& ev) {
        m_State = State::STOPPED;
        return true;
    }

    bool EngineManager::onResizeEvent(WindowResizedEvent &ev) {
        m_Renderer->getRenderCommand().refreshViewPort(ev.getWidth(), ev.getHeight());
        return true;
    }

    auto EngineManager::pushLayer(LayerStack::LayerPtr layer) -> void {
        m_LayerStack->addLayer(layer);
        layer->onAttach();
    }

    auto EngineManager::pushOverlay(LayerStack::LayerPtr overlay) -> void {
        m_LayerStack->addOverlay(overlay);
        overlay->onAttach();
    }

    auto EngineManager::shutDown() -> void {
        KATE_CORE_LOGGER_INFO("Shutting down kaTe Engine");

        m_LayerStack->shutDown();
        m_Window->shutDown();
    }

    auto EngineManager::getMainWindow() -> Window & {
        KT_ASSERT(m_Window, "Application main window is NULL");
        return *m_Window;
    }

    auto EngineManager::getInputManager() -> InputManager& {
        KT_ASSERT(m_InputManager, "Input manager is null");
        return *m_InputManager;
    }

    auto EngineManager::isRunning() -> bool {
        return m_State == State::RUNNING;
    }

    auto EngineManager::updateLayers() -> void {
        for (auto& layer : *m_LayerStack)
            layer->onUpdate();
    }

    void EngineManager::initOrthographicCamera() {
        KATE_CORE_LOGGER_INFO("kaTe Engine: Orthographic camera startup");
        m_Camera = std::make_unique<OrthographicCamera>(-1.0, 1.0, -1.0, 1.0);
        KT_ASSERT(m_Camera != nullptr, "OrthographicCamera is NULL");
    }

    void EngineManager::initRenderer() {
        KATE_CORE_LOGGER_INFO("kaTe Engine: Renderer startup");
        m_Renderer = std::make_unique<Renderer>();
        KT_ASSERT(m_Renderer != nullptr, "Renderer is NULL");
    }
}