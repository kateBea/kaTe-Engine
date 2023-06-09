/**
 * TestLayer.cc
 * Created by kate on 6/7/23.
 * */

#include <cmath>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>

#include "Renderer/Buffers/IndexBuffer.hh"
#include "Renderer/Buffers/VertexBuffer.hh"
#include "Renderer/Camera/OrthographicCamera.hh"
#include "Renderer/OpenGL/OpenGLShader.hh"
#include <Core/EngineManager.hh>
#include <Core/Layers/RuntimeLayer.hh>
#include <Platform/InputManager.hh>
#include <Renderer/Material/Texture2D.hh>

#include <Renderer/Renderer.hh>
#include <Renderer/RenderCommand.hh>

namespace kaTe {
    auto RuntimeLayer::onAttach() -> void {
        std::vector<float> data {
                // Positions                // Colors
                -0.5f, -0.5f, 0.0f,         1.0f, 0.0f, 0.0f, 1.0f,
                0.5f, -0.5f, 0.0f,          0.0f, 1.0f, 0.0f, 1.0f,
                0.0f, 0.5f, 0.0f,           0.0f, 0.0f, 1.0f, 1.0f,
        };

        std::vector<float> squareData {
                // Positions            // Texture coordinates
                -0.5f,  -0.5f, 0.0f,     0.0f, 0.0f, // bottom left
                 0.5f,  -0.5f, 0.0f,     1.0f, 0.0f, // bottom right
                 0.5f,   0.5f, 0.0f,     1.0f, 1.0f, // top right
                -0.5f,   0.5f, 0.0f,     0.0f, 1.0f, // top left
        };

        m_VertexBuffer.reset(VertexBuffer::createBuffer(data));
        m_IndexBuffer.reset(IndexBuffer::createBuffer({ 0, 1, 2 }));

        m_SquareVertexBuffer.reset(VertexBuffer::createBuffer(squareData));
        m_SquareIndexBuffer.reset(IndexBuffer::createBuffer({ 0, 1, 2, 2, 3, 0 }));

        m_VertexBuffer->setBufferLayout(BufferLayout{ { ShaderDataType::FLOAT3_TYPE, "a_Position" }, { ShaderDataType::FLOAT4_TYPE, "a_Color" } });
        m_SquareVertexBuffer->setBufferLayout(BufferLayout{ { ShaderDataType::FLOAT3_TYPE, "a_Position" }, { ShaderDataType::FLOAT2_TYPE, "a_TextureCoordinates" } });

        m_Shader = std::make_shared<OpenGLShader>("../assets/shaders/debugShaderVert.glsl", "../assets/shaders/debugShaderFrag.glsl");
        m_ColorShader = std::make_shared<OpenGLShader>("../assets/shaders/debugShaderVert.glsl", "../assets/shaders/colorShader.glsl");
        m_ColorTextureShader = std::make_shared<OpenGLShader>("../assets/shaders/textureVert.glsl", "../assets/shaders/textureFrag.glsl");
        m_Camera = std::make_shared<OrthographicCamera>(-1.6, 1.6, -0.9, 0.9);
        m_Texture.reset(Texture2D::createTexture("../assets/textures/stone_grass.png"));
        m_TextureTrans.reset(Texture2D::createTexture("../assets/textures/roots.png"));
        m_ColorTextureShader->setUniformInt("u_TextSampler", 0);
    }

    auto RuntimeLayer::onUpdate() -> void {
        auto engine{ EngineManager::getPtr() };

        static float posX{ 0.0f };
        static float posY{ 0.0f };
        static float rota{ 0.0f };

        auto deltaTime{ engine->getDeltaTime() };

        // Try using the event system instead
        if (InputManager::isKeyPressed(KT_KEY_LEFT)) m_CameraRotation += m_CameraRotationSpeed * deltaTime;
        if (InputManager::isKeyPressed(KT_KEY_RIGHT)) m_CameraRotation -= m_CameraRotationSpeed * deltaTime;

        if (InputManager::isKeyPressed(KT_KEY_A)) m_CameraPosition.x -= m_CameraMovementSpeed * deltaTime;
        if (InputManager::isKeyPressed(KT_KEY_D)) m_CameraPosition.x += m_CameraMovementSpeed * deltaTime;
        if (InputManager::isKeyPressed(KT_KEY_W)) m_CameraPosition.y += m_CameraMovementSpeed * deltaTime;
        if (InputManager::isKeyPressed(KT_KEY_S)) m_CameraPosition.y -= m_CameraMovementSpeed * deltaTime;

        if (InputManager::isKeyPressed(KT_KEY_J)) m_SquarePos.x -= m_CameraMovementSpeed * deltaTime;
        if (InputManager::isKeyPressed(KT_KEY_L)) m_SquarePos.x += m_CameraMovementSpeed * deltaTime;
        if (InputManager::isKeyPressed(KT_KEY_I)) m_SquarePos.y += m_CameraMovementSpeed * deltaTime;
        if (InputManager::isKeyPressed(KT_KEY_K)) m_SquarePos.y -= m_CameraMovementSpeed * deltaTime;

        m_ColorShader->setUniformVec4("u_Color", glm::vec4(m_SquareColor, 1.0));
        m_Camera->setPosition(m_CameraPosition.x, m_CameraPosition.y);
        m_Camera->setRotation(m_CameraRotation);

        constexpr static glm::mat4 IDENTITY_MAT(1.0);
        glm::mat4 scale{ glm::scale(IDENTITY_MAT, glm::vec3(0.1, 0.1, 0.1)) };

        double blue{ 0.5f };
        double time{ TimeManager::getTime(TimeUnit::NANOSECONDS) };
        double red{ (std::sin(time) + blue * 2) * blue };
        double green{ std::cos(time) + blue * 2 * blue };

        RenderCommand::setClearColor(red, green, blue, 1.0f);
        RenderCommand::clear((RendererAPI::BufferBit)(RendererAPI::OPEN_GL_COLOR_BUFFER_BIT | RendererAPI::OPEN_GL_DEPTH_BUFFER_BIT));

        Renderer::beginScene(m_Camera);
        // Grid. submit 20 rectangles
        for (UInt32_T countY{}; countY < 20; ++countY) {
            for (UInt32_T countX{}; countX < 20; ++countX) {
                glm::mat4 transform{ glm::translate(IDENTITY_MAT, m_SquarePos + glm::vec3(countX * 0.11, countY * -0.11, 0.0)) * scale };
                Renderer::submit(m_ColorShader, m_SquareVertexBuffer, m_SquareIndexBuffer, transform);
            }
        }

        // Big square
        m_Texture->bind();
        Renderer::submit(m_ColorTextureShader, m_SquareVertexBuffer, m_SquareIndexBuffer, glm::scale(IDENTITY_MAT, glm::vec3(1.5)) );

        m_TextureTrans->bind();
        Renderer::submit(m_ColorTextureShader, m_SquareVertexBuffer, m_SquareIndexBuffer, glm::scale(IDENTITY_MAT, glm::vec3(1.5)) );
        // Render Triangle
        // engine->getRenderer()->submit(m_Shader, m_VertexBuffer, m_IndexBuffer);
        Renderer::endScene();
    }

    auto RuntimeLayer::onImGuiRender() -> void {
        ImGui::Begin("Color");
        ImGui::ColorEdit3("Color", glm::value_ptr(m_SquareColor));
        ImGui::End();
    }

    auto RuntimeLayer::onEvent(Event &event) -> void {

    }

    auto RuntimeLayer::onDetach() -> void {

    }
}