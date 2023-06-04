#include <functional>
#include <memory>
#include <cmath>

// Third-party libraries
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Project headers
#include <Core/Events/AppEvents.hh>
#include <Core/Logger.hh>

#include <Tools/Application.hh>
#include <Tools/Common.hh>

#include <Core/Layers/ImGuiLayer.hh>
#include <Platform/Window/LinuxInputManager.hh>


namespace kaTe {
    static auto initOpenGLStuff(UInt32_T& vao, UInt32_T& vbo, UInt32_T& veo) -> void {
        float data[] {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f, 0.5f, 0.0f
        };

        UInt32_T indices[]{ 0, 1, 2 };

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

        glGenBuffers(1, &veo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, veo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

    }

    auto Application::init() -> void {
        KATE_APP_LOGGER_INFO("Initializing kaTe Engine");

        initWindow();
        initLayerStack();
        initInputManager();

        // temporary. may be done by the renderer
        initOpenGLStuff(m_Vao, m_Vbo, m_Veo);

        m_Shader.load("../assets/shaders/debugShaderVert.glsl", "../assets/shaders/debugShaderFrag.glsl");


        KATE_APP_LOGGER_DEBUG("Finished kaTe Engine initialization");
    }

    auto Application::loop() -> void {
        KATE_CORE_LOGGER_INFO("Entering main loop kaTe Engine");

        while (m_State == State::RUNNING) {
            SWAP_BG_COLOR_INTERVAL();

            m_Shader.use();
            glBindVertexArray(m_Vao);
            glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

            for (auto& layer : *m_LayerStack)
                layer->onUpdate();
            m_Window->onUpdate();
        }
    }

    auto Application::initWindow() -> void {
        KATE_APP_LOGGER_INFO("Initializing Application::Window kaTe Engine");
        m_Window = std::make_unique<kateGLFWwindow>();
        KT_ASSERT(m_Window != nullptr, "Window is NULL");

        m_Window->init();
        m_Window->setEventCallback(KT_BIND_EVENT_FUNC(Application::onEvent));
    }

    auto Application::initLayerStack() -> void {
        KATE_APP_LOGGER_INFO("Initializing Application::LayerStack kaTe Engine");
        m_LayerStack = std::make_unique<LayerStack>();
        KT_ASSERT(m_LayerStack != nullptr, "Layer Stack is NULL");

        m_LayerStack->init();

        SPtr_T<ImGuiLayer> temp{ std::make_shared<ImGuiLayer>() };
        temp->onAttach();
        m_LayerStack->addLayer(temp);
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
}