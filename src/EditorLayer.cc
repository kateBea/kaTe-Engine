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

        FrameBufferCreateInfo createInfo{};

        createInfo.width = window.GetWidth();
        createInfo.height = window.GetHeight();
        createInfo.samples = 1;

        // Panel data setup
        m_SettingsPanelInfo = std::make_shared<SettingsPanelData>();
        m_SettingsPanelInfo->ClearColor = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);

        m_ScenePanelInfo = std::make_shared<ScenePanelData>();
        m_ScenePanelInfo->SceneFrameBuffer = FrameBuffer::CreatFrameBuffer(createInfo);
        m_ScenePanelInfo->Viewport = std::make_unique<Scene>();


        m_StatsPanelInfo = std::make_shared<StatsPanelData>();



        // Scene pre setup
        auto ent1{ Scene::CreateEntity("RedColoredSquare", m_ScenePanelInfo->Viewport) };
        auto ent2{ Scene::CreateEntity("GreenColoredSquare", m_ScenePanelInfo->Viewport) };
        auto cam{ Scene::CreateEntity("MainCamera", m_ScenePanelInfo->Viewport) };

        ent1.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.8f, 0.2f, 0.15f, 1.0f });
        ent2.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.2f, 0.8f, 0.25f, 0.5f });

        ent1.GetComponent<TransformComponent>().SetTranslation({0.0f, 0.0f, .01f});
        ent2.GetComponent<TransformComponent>().SetTranslation({1.0f, 0.0f, 1.0f});

        ent1.GetComponent<TransformComponent>().SetRotation({0.0f, 0.0f, 45.0f});

        double aspect{ window.GetWidth() / (double)window.GetHeight() };
        m_MainCamera = std::make_shared<SceneCamera>(glm::ortho(-aspect, aspect, -1.0, 1.0));
        cam.AddComponent<CameraComponent>(m_MainCamera);
        cam.AddComponent<NativeScriptComponent>();

        // Panels setup
        m_HierarchyPanel = std::make_shared<HierarchyPanel>(m_ScenePanelInfo->Viewport);
        m_InspectorPanel = std::make_shared<InspectorPanel>(m_HierarchyPanel);
        m_SettingsPanel = std::make_shared<SettingsPanel>(m_SettingsPanelInfo);
        m_ScenePanel = std::make_shared<ScenePanel>(m_ScenePanelInfo);
        m_StatsPanel = std::make_shared<StatsPanel>(m_StatsPanelInfo);

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
        m_ScenePanelInfo->SceneFrameBuffer->Bind();
        RenderCommand::SetClearColor(m_SettingsPanelInfo->ClearColor);
        RenderCommand::Clear((RendererAPI::BufferBit)(RendererAPI::OPEN_GL_COLOR_BUFFER_BIT |
                                                      RendererAPI::OPEN_GL_DEPTH_BUFFER_BIT));

        m_ScenePanelInfo->Viewport->OnUpdate();
        m_ScenePanelInfo->SceneFrameBuffer->Unbind();
    }

    auto EditorLayer::OnEvent(Event &event) -> void {
        m_SettingsPanel->OnEvent(event);
        m_HierarchyPanel->OnEvent(event);
        m_InspectorPanel->OnEvent(event);
        m_ScenePanel->OnEvent(event);
        m_StatsPanel->OnEvent(event);
    }

    auto EditorLayer::OnImGuiRender() -> void {
        Editor::OnDockSpaceUpdate(m_DockEditorData);

        m_SettingsPanel->MakeVisible(m_DockEditorData.SettingPanelVisible);
        m_HierarchyPanel->MakeVisible(m_DockEditorData.HierarchyPanelVisible);
        m_InspectorPanel->MakeVisible(m_DockEditorData.InspectorPanelVisible);
        m_ScenePanel->MakeVisible(m_DockEditorData.ScenePanelVisible);
        m_StatsPanel->MakeVisible(m_DockEditorData.StatsPanelVisible);

        m_SettingsPanel->OnUpdate();
        m_HierarchyPanel->OnUpdate();
        m_InspectorPanel->OnUpdate(); // the inspector panel comes after because it needs data from the current state of the hierarchy
        m_ScenePanel->OnUpdate();
        m_StatsPanel->OnUpdate();

        if (m_DockEditorData.ApplicationCloseFlag)
            Application::GetPtr()->Stop();
    }
}
