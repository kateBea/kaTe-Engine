#include <functional>
#include <memory>
#include <cmath>

// Project headers
#include <Core/Events/AppEvents.hh>
#include <Core/Logger.hh>

#include "Core/Application.hh"
#include <Tools/Common.hh>

#include "Core/Layers/ImGuiLayer.hh"
#include "Renderer/Renderer2D.hh"
#include <Platform/InputManagerGLFW.hh>

#include <Renderer/Renderer.hh>
#include <Renderer/RenderCommand.hh>


namespace kaTe {
    auto Application::Init() -> void {
        TimeManager::Init();
        KATE_CORE_LOGGER_DEBUG("Initializing kaTe Engine {}",
                              TimeManager::ToString(TimeManager::GetTime()));
        m_MainWindow = std::make_unique<CrossPlatformWindow>();
        m_LayerStack = std::make_unique<LayerStack>();

        KT_ASSERT(m_MainWindow != nullptr, "Window is NULL");
        m_MainWindow->init();
        m_MainWindow->setEventCallback(KT_BIND_EVENT_FUNC(Application::OnEvent));// Should probably not be done here
        KT_ASSERT(m_LayerStack != nullptr, "Layer Stack is NULL");
        m_LayerStack->init();

        Renderer::Init();
        Renderer2D::Init();
        RenderCommand::Init();
        InputManager::init();

        KATE_CORE_LOGGER_DEBUG("Finished kaTe Engine initialization {}",
                               TimeManager::ToString(TimeManager::GetTime()));
    }

    // Should probably not be here
    auto Application::OnEvent(Event& event) -> void {
        KATE_CORE_LOGGER_TRACE("{}", event.DisplayData());

        EventDispatcher evDis{ event };
        if (evDis.forward<WindowCloseEvent>(KT_BIND_EVENT_FUNC(Application::OnWindowClose)))
            KATE_CORE_LOGGER_TRACE("HANDLED {}", event.DisplayData());

        if (evDis.forward<WindowResizedEvent>(KT_BIND_EVENT_FUNC(Application::OnResizeEvent)))
            KATE_CORE_LOGGER_TRACE("HANDLED {}", event.DisplayData());

        for (auto it{ m_LayerStack->rbegin() }; it != m_LayerStack->rend(); ++it) {
            (*it)->onEvent(event);
            if (event.IsHandled())
                break;
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent& event) {
        m_State = State::STOPPED;

        // We don't want to propagate this event
        return true;
    }

    bool Application::OnResizeEvent(WindowResizedEvent& event) {
        m_MainWindowMinimized = event.GetWidth() == 0 || event.GetHeight() == 0;
        m_State = m_MainWindowMinimized ? State::IDLE : State::RUNNING;
        RenderCommand::UpdateViewPort(0, 0, event.GetWidth(), event.GetHeight());
        return false;
    }

    auto Application::PushLayer(std::shared_ptr<Layer> layer) -> void {
        m_LayerStack->addLayer(layer);
        layer->onAttach();
    }

    auto Application::PushOverlay(std::shared_ptr<Layer> overlay) -> void {
        m_LayerStack->addOverlay(overlay);
        overlay->onAttach();
    }

    auto Application::ShutDown() -> void {
        KATE_CORE_LOGGER_INFO("Shutting down kaTe Engine");

        InputManager::shutDown();
        RenderCommand::ShutDown();
        Renderer::ShutDown();

        m_LayerStack->shutDown();
        m_MainWindow->shutDown();
    }

    auto Application::GetMainWindow() -> Window & {
        KT_ASSERT(m_MainWindow, "Application main window is NULL");
        return *m_MainWindow;
    }

    auto Application::IsRunning() -> bool {
        return m_State == State::RUNNING || m_State == State::IDLE;
    }

    auto Application::UpdateState() -> void {
        TimeManager::UpdateDeltaTime();

        if (!m_MainWindowMinimized) {
            for (auto& layer : *m_LayerStack)
                layer->onUpdate();
        }

        ImGuiLayer::beginFrame();
        for (auto& layer : *m_LayerStack)
            layer->onImGuiRender();
        ImGuiLayer::endFrame();
        m_MainWindow->onUpdate();
    }
}