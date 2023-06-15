//
// Created by kate on 6/12/23.
//
#include <memory>

#include <imgui.h>

#include <glm/gtc/type_ptr.hpp>

#include "Renderer/OpenGL/OpenGLTexture2D.hh"
#include <Core/Logger.hh>
#include <Core/Application.hh>
#include <Core/Layers/SandBox2D.hh>
#include <Core/TimeManager.hh>

#include <Platform/InputManager.hh>
#include <Renderer/Material/Texture2D.hh>
#include <Renderer/RenderCommand.hh>
#include <Renderer/Renderer2D.hh>

#include <Core/Timer.hh>

namespace kaTe {
    auto SandBox2D::onAttach() -> void {
        Window& window{ Application::Get().GetMainWindow() };
        m_CameraController = std::make_shared<OrthographicCameraController>(window.getWidth(), window.getHeight(), true, nullptr);
        m_Texture = Texture2D::CreateTexture("../assets/textures/roots.png");
    }

    auto SandBox2D::onDetach() -> void {

    }

    auto SandBox2D::onUpdate() -> void {
        KT_PROFILE_SCOPE();
        auto deltaTime{ TimeManager::GetDeltaTime() };

        RenderCommand::SetClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        RenderCommand::Clear((RendererAPI::BufferBit)(RendererAPI::OPEN_GL_COLOR_BUFFER_BIT | RendererAPI::OPEN_GL_DEPTH_BUFFER_BIT));

        if (InputManager::isKeyPressed(KT_KEY_J))
            m_FirstSquarePos.x -= m_FirstSquareMovementSpeed * deltaTime;
        if (InputManager::isKeyPressed(KT_KEY_L))
            m_FirstSquarePos.x += m_FirstSquareMovementSpeed * deltaTime;
        if (InputManager::isKeyPressed(KT_KEY_I))
            m_FirstSquarePos.y += m_FirstSquareMovementSpeed * deltaTime;
        if (InputManager::isKeyPressed(KT_KEY_K))
            m_FirstSquarePos.y -= m_FirstSquareMovementSpeed * deltaTime;

        m_CameraController->OnUpdate();
        Renderer2D::BeginScene(m_CameraController->GetCamera());
        Renderer2D::DrawQuad(m_FirstSquarePos, { 1.0f, 1.0f }, { m_SquareColor, 1.0f }, 45 * TimeManager::GetTime(TimeUnit::SECONDS));
        Renderer2D::DrawQuad(m_SecondSquarePos, { 1.0f, 1.0f }, { 0.8f, 0.5f, 0.1f, 1.0f }, 90 * TimeManager::GetTime(TimeUnit::SECONDS), m_Texture);
        Renderer2D::EndScene();

    }

    auto SandBox2D::onEvent(Event &event) -> void {
        m_CameraController->OnEvent(event);
    }

    auto SandBox2D::onImGuiRender() -> void {
        ImGui::Begin("Color");
        ImGui::ColorEdit3("Color", glm::value_ptr(m_SquareColor));
        ImGui::End();
    }
}
