/**
 * TestLayer.cc
 * Created by kate on 6/7/23.
 * */

#include <cmath>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>

#include "Core/Layers/TestLayer.hh"
#include "Renderer/Buffers/IndexBuffer.hh"
#include "Renderer/Buffers/VertexBuffer.hh"
#include "Renderer/Camera/OrthographicCamera.hh"
#include "Renderer/OpenGL/OpenGLShader.hh"
#include "Renderer/OpenGL/OpenGLTexture2D.hh"
#include <Core/Application.hh>
#include <Platform/InputManager.hh>
#include <Renderer/Material/Texture2D.hh>

#include <Renderer/Renderer.hh>
#include <Renderer/RenderCommand.hh>

namespace kaTe {
    auto TestLayer::onAttach() -> void {
        Window& window{ Application::Get().GetMainWindow() };

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

        m_VertexBuffer = VertexBuffer::CreateBuffer(data);
        m_IndexBuffer = IndexBuffer::CreateBuffer({0, 1, 2});

        m_SquareVertexBuffer = VertexBuffer::CreateBuffer(squareData);
        m_SquareIndexBuffer = IndexBuffer::CreateBuffer({0, 1, 2, 2, 3, 0});
        m_Texture = Texture2D::CreateTexture("../assets/textures/stone_grass.png");
        m_TextureTrans = Texture2D::CreateTexture("../assets/textures/roots.png");

        m_VertexBuffer->SetBufferLayout(BufferLayout{{ShaderDataType::FLOAT3_TYPE, "a_Position"}, {ShaderDataType::FLOAT4_TYPE, "a_Color"}});
        m_SquareVertexBuffer->SetBufferLayout(BufferLayout{{ShaderDataType::FLOAT3_TYPE, "a_Position"}, {ShaderDataType::FLOAT2_TYPE, "a_TextureCoordinates"}});

        m_ColorShader = std::make_shared<OpenGLShader>("../assets/shaders/debugShaderVert.glsl", "../assets/shaders/colorShader.glsl");
        m_ColorTextureShader = std::make_shared<OpenGLShader>("../assets/shaders/textureVert.glsl", "../assets/shaders/textureFrag.glsl");
        m_CameraController = std::make_shared<OrthographicCameraController>(window.getWidth(), window.getHeight(), true, nullptr);
        m_ColorTextureShader->setUniformInt("u_TextSampler", 0);
    }

    auto TestLayer::onUpdate() -> void {
        auto deltaTime{ TimeManager::GetDeltaTime() };
        if (InputManager::isKeyPressed(KT_KEY_J)) m_SquarePos.x -= m_CameraMovementSpeed * deltaTime;
        if (InputManager::isKeyPressed(KT_KEY_L)) m_SquarePos.x += m_CameraMovementSpeed * deltaTime;
        if (InputManager::isKeyPressed(KT_KEY_I)) m_SquarePos.y += m_CameraMovementSpeed * deltaTime;
        if (InputManager::isKeyPressed(KT_KEY_K)) m_SquarePos.y -= m_CameraMovementSpeed * deltaTime;

        m_CameraController->OnUpdate();
        m_ColorShader->setUniformVec4("u_Color", glm::vec4(m_SquareColor, 1.0));

        constexpr static glm::mat4 IDENTITY_MAT(1.0);
        glm::mat4 scale{ glm::scale(IDENTITY_MAT, glm::vec3(0.1, 0.1, 0.1)) };

        double blue{ 0.5f };
        double time{ TimeManager::GetTime(TimeUnit::NANOSECONDS) };
        double red{ (std::sin(time) + blue * 2) * blue };
        double green{ std::cos(time) + blue * 2 * blue };

        RenderCommand::SetClearColor(red, green, blue, 1.0f);
        RenderCommand::Clear((RendererAPI::BufferBit)(RendererAPI::OPEN_GL_COLOR_BUFFER_BIT | RendererAPI::OPEN_GL_DEPTH_BUFFER_BIT));

        Renderer::BeginScene(m_CameraController->GetCamera());
        // Grid. submit 20 rectangles
        for (UInt32_T countY{}; countY < 20; ++countY) {
            for (UInt32_T countX{}; countX < 20; ++countX) {
                glm::mat4 transform{ glm::translate(IDENTITY_MAT, m_SquarePos + glm::vec3(countX * 0.11, countY * -0.11, -0.1)) * scale };
                Renderer::Submit(m_ColorShader, m_SquareVertexBuffer, m_SquareIndexBuffer, transform);
            }
        }

        // Big square
        m_Texture->Bind();
        Renderer::Submit(m_ColorTextureShader, m_SquareVertexBuffer, m_SquareIndexBuffer, glm::scale(IDENTITY_MAT, glm::vec3(1.5)));

        m_TextureTrans->Bind();
        Renderer::Submit(m_ColorTextureShader, m_SquareVertexBuffer, m_SquareIndexBuffer, glm::scale(IDENTITY_MAT, glm::vec3(1.5)));
        Renderer::EndScene();
    }

    auto TestLayer::onImGuiRender() -> void {
        ImGui::Begin("Color");
        ImGui::ColorEdit3("Color", glm::value_ptr(m_SquareColor));
        ImGui::End();
    }

    auto TestLayer::onEvent(Event &event) -> void {
        m_CameraController->OnEvent(event);
    }

    auto TestLayer::onDetach() -> void {

    }
}