/**
 * TestLayer.cc
 * Created by kate on 6/7/23.
 * */

#include <Core/EngineManager.hh>
#include <Core/Layers/RuntimeLayer.hh>
#include <Platform/InputManager.hh>
#include <Renderer/IndexBuffer.hh>
#include <Renderer/OrthographicCamera.hh>
#include <Renderer/Shader.hh>
#include <Renderer/VertexBuffer.hh>

namespace kaTe {
    auto RuntimeLayer::onAttach() -> void {
        std::vector<float> data {
                // Positions                // Colors
                -0.5f, -0.5f, 0.0f,         1.0f, 0.0f, 1.0f, 1.0f,
                0.5f, -0.5f, 0.0f,          0.0f, 0.0f, 1.0f, 1.0f,
                0.0f, 0.5f, 0.0f,           1.0f, 1.0f, 0.0f, 1.0f,
        };

        std::vector<float> squareData {
                // Positions                // Colors
                -0.5f, 0.5f, 0.0f,         1.0f, 0.0f, 1.0f, 1.0f,
                0.5f, 0.5f, 0.0f,          0.0f, 0.0f, 1.0f, 1.0f,
                -0.5f, -0.5f, 0.0f,           1.0f, 1.0f, 0.0f, 1.0f,
                0.5f, -0.5f, 0.0f,           0.0f, 1.0f, 1.0f, 1.0f,
        };

        m_VertexBuffer.reset(VertexBuffer::createBuffer(data));
        m_IndexBuffer.reset(IndexBuffer::createBuffer({ 0, 1, 2 }));

        m_SquareVertexBuffer.reset(VertexBuffer::createBuffer(squareData));
        m_SquareIndexBuffer.reset(IndexBuffer::createBuffer({ 0, 1, 2, 1, 2, 3 }));

        m_VertexBuffer->setBufferLayout(BufferLayout{ { ShaderDataType::FLOAT3_TYPE, "a_Position" }, { ShaderDataType::FLOAT4_TYPE, "a_Color" } });
        m_SquareVertexBuffer->setBufferLayout(BufferLayout{ { ShaderDataType::FLOAT3_TYPE, "a_Position" }, { ShaderDataType::FLOAT4_TYPE, "a_Color" } });

        m_Shader.reset(new Shader("../assets/shaders/debugShaderVert.glsl", "../assets/shaders/debugShaderFrag.glsl"));
        m_Camera = std::make_shared<OrthographicCamera>(-1.0, 1.0, -1.0, 1.0);
    }

    auto RuntimeLayer::onUpdate() -> void {
        auto engine{ EngineManager::getPtr() };

        static float posX{ 0.0f };
        static float posY{ 0.0f };
        static float rota{ 0.0f };

        // Try using the event system instead
        if (InputManager::isKeyPressed(KT_KEY_LEFT)) m_CameraRotation -= 1.0f * m_CameraRotationSpeed;
        if (InputManager::isKeyPressed(KT_KEY_RIGHT)) m_CameraRotation += 1.0f * m_CameraRotationSpeed;;

        if (InputManager::isKeyPressed(KT_KEY_A)) m_CameraPosition.x += 0.01f * m_CameraMovementSpeed;
        if (InputManager::isKeyPressed(KT_KEY_D)) m_CameraPosition.x -= 0.01f * m_CameraMovementSpeed;
        if (InputManager::isKeyPressed(KT_KEY_W)) m_CameraPosition.y -= 0.01f * m_CameraMovementSpeed;
        if (InputManager::isKeyPressed(KT_KEY_S)) m_CameraPosition.y += 0.01f * m_CameraMovementSpeed;

        m_Camera->setPosition(m_CameraPosition.x, m_CameraPosition.y);
        m_Camera->setRotation(m_CameraRotation);

        float time = glfwGetTime();
        float blue = 0.5f;
        float red = (std::sin(time) + blue * 2) * blue;
        float green = (std::cos(time) + blue * 2) * blue;
        engine->getRenderer()->getRenderCommand().setClearColor(red, green, blue, 1.0f);
        engine->getRenderer()->getRenderCommand().clear((RendererAPI::BufferBit)(RendererAPI::OPEN_GL_COLOR_BUFFER_BIT |
                                                                                 RendererAPI::OPEN_GL_DEPTH_BUFFER_BIT));
        engine->getRenderer()->beginScene(m_Camera);
        engine->getRenderer()->submit(m_Shader, m_SquareVertexBuffer, m_SquareIndexBuffer);
        engine->getRenderer()->submit(m_Shader, m_VertexBuffer, m_IndexBuffer);
        engine->getRenderer()->endScene();
    }

    auto RuntimeLayer::onEvent(Event &event) -> void {

    }

    auto RuntimeLayer::onDetach() -> void {

    }
}