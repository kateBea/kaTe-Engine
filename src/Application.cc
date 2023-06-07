#include <functional>
#include <memory>
#include <cmath>

// Third-party libraries
#include <GLFW/glfw3.h>

// Project headers
#include <Core/Events/AppEvents.hh>
#include <Core/Logger.hh>

#include <Tools/Application.hh>
#include <Tools/Common.hh>

#include <Core/Layers/ImGuiLayer.hh>
#include <Platform/Window/LinuxInputManager.hh>

#include <Renderer/Renderer.hh>
#include <Renderer/RenderCommand.hh>


namespace kaTe {
    auto Application::init() -> void {
        KATE_APP_LOGGER_INFO("Initializing kaTe Engine");

        initWindow();
        initLayerStack();
        initInputManager();

        // temporary. may be done by the renderer
        {
            std::vector<float> data {
                    // Positions                // Colors
                -0.5f, -0.5f, 0.0f,         1.0f, 0.0f, 1.0f, 1.0f,
                0.5f, -0.5f, 0.0f,          0.0f, 0.0f, 1.0f, 1.0f,
                0.0f, 0.5f, 0.0f,           1.0f, 1.0f, 0.0f, 1.0f,
            };

            m_VertexBuffer.reset(VertexBuffer::createBuffer(data));
            m_VertexIndexBuffer.reset(IndexBuffer::createBuffer({ 0, 1, 2} ));

            m_Shader.reset(new Shader("../assets/shaders/debugShaderVert.glsl", "../assets/shaders/debugShaderFrag.glsl"));
        }

        m_Camera.reset(new OrthographicCamera(-1.0, 1.0, -1.0, 1.0));

        KATE_APP_LOGGER_DEBUG("Finished kaTe Engine initialization");
    }

    auto Application::loop() -> void {
        KATE_CORE_LOGGER_INFO("Entering main loop kaTe Engine");
        std::vector<float> data {
                // Positions                // Colors
                0.0f, 0.5f, 0.0f,         1.0f, 0.0f, 1.0f, 1.0f,
                1.0f, 0.5f, 0.0f,          0.0f, 0.0f, 1.0f, 1.0f,
                0.5f, 1.0f, 0.0f,           1.0f, 1.0f, 0.0f, 1.0f,
        };

        std::vector<float> data2 {
                // Positions                // Colors
                0.0f, 0.5f, 0.0f,         1.0f, 0.0f, 1.0f, 1.0f,
                -1.0f, 0.5f, 0.0f,          0.0f, 0.0f, 1.0f, 1.0f,
                -0.5f, 1.0f, 0.0f,           1.0f, 1.0f, 0.0f, 1.0f,
        };

        SPtr_T<VertexBuffer> vertexBuffer1{};
        SPtr_T<VertexBuffer> vertexBuffer2{};
        vertexBuffer1.reset(VertexBuffer::createBuffer(data));
        vertexBuffer2.reset(VertexBuffer::createBuffer(data2));

        m_VertexBuffer->setBufferLayout(BufferLayout{{ ShaderDataType::FLOAT3_TYPE, "a_Position" }, { ShaderDataType::FLOAT4_TYPE, "a_Color" }});
        vertexBuffer1->setBufferLayout(BufferLayout{{ ShaderDataType::FLOAT3_TYPE, "a_Position" }, { ShaderDataType::FLOAT4_TYPE, "a_Color" }});
        vertexBuffer2->setBufferLayout(BufferLayout{{ ShaderDataType::FLOAT3_TYPE, "a_Position" }, { ShaderDataType::FLOAT4_TYPE, "a_Color" }});

        // Temporary
        RenderCommand rc{};

        while (m_State == State::RUNNING) {
            float time = glfwGetTime();
            float blue = 0.5f;
            float red = (std::sin(time) + blue * 2) * blue;
            float green = (std::cos(time) + blue * 2) * blue;

            RenderCommand::setClearColor(red, green, blue, 1.0f);
            RenderCommand::clear((RendererAPI::BufferBit)(RendererAPI::OPEN_GL_COLOR_BUFFER_BIT | RendererAPI::OPEN_GL_DEPTH_BUFFER_BIT));

            Renderer::beginScene(m_Camera);
            Renderer::submit(m_Shader, m_VertexBuffer, m_VertexIndexBuffer);
            Renderer::submit(m_Shader, vertexBuffer1, m_VertexIndexBuffer);
            Renderer::submit(m_Shader, vertexBuffer2, m_VertexIndexBuffer);
            Renderer::endScene();

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