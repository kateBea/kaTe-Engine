#include <functional>
#include <memory>
#include <cmath>

// Project headers
#include <Core/Events/AppEvents.hh>
#include <Core/Logger.hh>

#include "Core/EngineManager.hh"
#include <Tools/Common.hh>

#include <Core/Layers/ImGuiLayer.hh>
#include <Platform/InputManagerGLFW.hh>

#include <Renderer/Renderer.hh>
#include <Renderer/RenderCommand.hh>


namespace kaTe {
    auto EngineManager::init() -> void {
        KATE_CORE_LOGGER_INFO("Initializing kaTe Engine ----------------------------------------");

        m_MainWindow = std::make_unique<WindowGLFW>();
        m_LayerStack = std::make_unique<LayerStack>();

        KT_ASSERT(m_MainWindow != nullptr, "Window is NULL");
        m_MainWindow->init();
        m_MainWindow->setEventCallback(KT_BIND_EVENT_FUNC(EngineManager::onEvent));// Should probably not be done here
        KT_ASSERT(m_LayerStack != nullptr, "Layer Stack is NULL");
        m_LayerStack->init();

        Renderer::init();
        RenderCommand::init();
        InputManager::init();

        KATE_CORE_LOGGER_DEBUG("Finished kaTe Engine initialization ----------------------------");
    }

    auto EngineManager::initWindow() -> void {
        KATE_CORE_LOGGER_INFO("kaTe Engine: Main Window initialization");
        m_MainWindow = std::make_unique<WindowGLFW>();
        KT_ASSERT(m_MainWindow != nullptr, "Window is NULL");
        m_MainWindow->init();
        // Should probably not be done here
        m_MainWindow->setEventCallback(KT_BIND_EVENT_FUNC(EngineManager::onEvent));
    }

    auto EngineManager::initLayerStack() -> void {
        KATE_CORE_LOGGER_INFO("kaTe Engine: Layer Stack initialization");
        m_LayerStack = std::make_unique<LayerStack>();
        KT_ASSERT(m_LayerStack != nullptr, "Layer Stack is NULL");
        m_LayerStack->init();
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
        RenderCommand::refreshViewPort(ev.getWidth(), ev.getHeight());
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

        InputManager::shutDown();
        RenderCommand::shutDown();
        Renderer::shutDown();

        m_LayerStack->shutDown();
        m_MainWindow->shutDown();
    }

    auto EngineManager::getMainWindow() -> Window & {
        KT_ASSERT(m_MainWindow, "Application main window is NULL");
        return *m_MainWindow;
    }

    auto EngineManager::isRunning() -> bool {
        return m_State == State::RUNNING;
    }

    auto EngineManager::updateState() -> void {
        m_DeltaTime.updateDeltaTime();
        for (auto& layer : *m_LayerStack)
            layer->onUpdate();

        ImGuiLayer::beginFrame();
        for (auto& layer : *m_LayerStack)
            layer->onImGuiRender();
        ImGuiLayer::endFrame();

        m_MainWindow->onUpdate();
    }
}