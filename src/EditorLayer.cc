//
// Created by kate on 6/12/23.
//
#include <memory>

#include <imgui.h>

#include <glm/gtc/type_ptr.hpp>

#include "Editor/EditorLayer.hh"
#include <Core/Application.hh>

#include <Platform/InputManager.hh>
#include <Renderer/RenderCommand.hh>
#include <Renderer/Renderer2D.hh>
#include <Renderer/Buffers/FrameBuffer.hh>

#include <Editor/Editor.hh>
#include <Scene/Scene.hh>

namespace kaTe {
    auto EditorLayer::OnAttach() -> void {
        Window& window{ Application::Get().GetMainWindow() };
        m_CameraController = std::make_shared<OrthographicCameraController>(window.GetWidth(), window.GetHeight(), true, nullptr);
        m_VerticalSyncEnabled = window.IsVSyncEnabled();

        FrameBufferCreateInfo createInfo{};

        createInfo.width = window.GetWidth();
        createInfo.height = window.GetHeight();
        createInfo.samples = 1;

        m_FrameBuffer = FrameBuffer::CreatFrameBuffer(createInfo);

        // Scene setup
        m_ActiveScene = std::make_unique<Scene>();
        m_SquareEntity = Scene::CreateEntity("RedColoredSquare", m_ActiveScene);
        auto  other = Scene::CreateEntity("GreenColoredSquare", m_ActiveScene);
        m_MainCamEntity = Scene::CreateEntity("MainCamera", m_ActiveScene);

        m_SquareEntity.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.8f, 0.2f, 0.15f, 1.0f });
        other.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.2f, 0.8f, 0.25f, 0.5f });

        m_SquareEntity.GetComponent<TransformComponent>().SetTranslation({0.0f, 0.0f, .01f});
        other.GetComponent<TransformComponent>().SetTranslation(m_Position);

        m_SquareEntity.GetComponent<TransformComponent>().SetRotation({0.0f, 0.0f, 45.0f});

        double aspect{ window.GetWidth() / (double)window.GetHeight() };
        m_MainCamera = std::make_shared<SceneCamera>(glm::ortho(-aspect, aspect, -1.0, 1.0));
        m_MainCamEntity.AddComponent<CameraComponent>(m_MainCamera);
        m_MainCamEntity.AddComponent<NativeScriptComponent>();

        m_HierarchyPanel = std::make_shared<SceneHierarchyPanel>(m_ActiveScene);
        m_InspectorPanel = std::make_shared<InspectorPanel>(m_HierarchyPanel);

        {
            // scripting test
            // TODO: does not compile
            //m_MainCamEntity.GetComponent<NativeScriptComponent>().Bind<CameraController>();
        }
    }

    auto EditorLayer::OnDetach() -> void {

    }

    auto EditorLayer::OnUpdate() -> void {
        // Pre Setup
        m_FrameBuffer->Bind();
        RenderCommand::SetClearColor(m_ClearColor);
        RenderCommand::Clear((RendererAPI::BufferBit)(RendererAPI::OPEN_GL_COLOR_BUFFER_BIT | RendererAPI::OPEN_GL_DEPTH_BUFFER_BIT));

        // will probably change to m_ViewportPanel->IsFocused() or an array of panels
        // since we may probably abstract imgui windows into panels for the docking space
        if (m_ViewportIsFocused)
            // Only update the state of the camera controller when the viewport panel is focused
            m_CameraController->OnUpdate();

        m_ActiveScene->OnUpdate();

        m_FrameBuffer->Unbind();
    }

    auto EditorLayer::OnEvent(Event &event) -> void {
        // Only forward events to the camera controller when the viewport panel is focused
        if (m_ViewportIsFocused)
            m_CameraController->OnEvent(event);

        m_HierarchyPanel->OnEvent(event);
    }

    auto EditorLayer::OnImGuiRender() -> void {
        Editor::DockControlFlags flags{};

        Editor::DisplayDockSpace(flags);
        ImGui::Begin("Editor");
        ImGui::ColorEdit3("Clear Color", glm::value_ptr(m_ClearColor));
        ImGui::DragFloat3("Pos red", glm::value_ptr(m_Position));
        ImGui::ColorPicker4("Color green", glm::value_ptr(m_Color));
        ImGui::End();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0,0});
        ImGui::Begin("Scene");
        m_ViewportIsFocused = ImGui::IsWindowFocused();
        m_ViewportIsHovered = ImGui::IsWindowHovered();

        Application::GetPtr()->BlockImGuiLayerEvents(!m_ViewportIsFocused || !m_ViewportIsHovered);

        auto viewPortDimensions{ ImGui::GetContentRegionAvail() };

        if (m_ViewPortWidth != viewPortDimensions.x || m_ViewPortHeight != viewPortDimensions.y) {
            m_FrameBuffer->Resize((UInt32_T)viewPortDimensions.x, (UInt32_T)viewPortDimensions.y);
            m_ViewPortHeight = viewPortDimensions.y;
            m_ViewPortWidth = viewPortDimensions.x;
            m_CameraController->AdjustViewport((UInt32_T)viewPortDimensions.x, (UInt32_T)viewPortDimensions.y);
            m_ActiveScene->OnViewPortResize((UInt32_T)viewPortDimensions.x, (UInt32_T)viewPortDimensions.y);
        }

        ImTextureID textId{ reinterpret_cast<ImTextureID>(m_FrameBuffer->GetColorAttachmentId()) };
        float frameWidth{ static_cast<float>(m_FrameBuffer->GetFrameBufferProperties().width) };
        float frameHeight{ static_cast<float>(m_FrameBuffer->GetFrameBufferProperties().height) };
        ImGui::Image((ImTextureID)textId, ImVec2{frameWidth, frameHeight}, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

        ImGui::End();
        ImGui::PopStyleVar();

        {
            // TODO: move other imgui windows to its panels
            // Panel render
            m_HierarchyPanel->OnUpdate();

            // inscpector panel comes after because it needs data from
            // the current state of the hierarchy
            m_InspectorPanel->OnUpdate();
        }

        ImGui::Begin("Statistics");
        ImGui::Text("Draw calls count: %d", Renderer2D::QueryDrawCallsCount());
        ImGui::Text("Quad count: %d", Renderer2D::QueryQuadCount());
        ImGui::Text("Index count: %d", Renderer2D::QueryIndexCount());
        ImGui::Text("Vertex count: %d", Renderer2D::QueryVertexCount());
        ImGui::Text("Frame rate: %.1f", ImGui::GetIO().Framerate);

        if (ImGui::Checkbox("VSync Enabled", &m_VerticalSyncEnabled)) {
            if (m_VerticalSyncEnabled)
                Application::Get().GetMainWindow().EnableVSync();
            else
                Application::Get().GetMainWindow().DisableVSync();
        }

        ImGui::End();

        if (flags.applicationCloseFlag)
            Application::GetPtr()->Stop();
    }
}
