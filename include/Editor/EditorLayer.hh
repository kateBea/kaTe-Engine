//
// Created by kate on 6/12/23.
//

#ifndef KATE_ENGINE_EDITOR_LAYER_HH
#define KATE_ENGINE_EDITOR_LAYER_HH

#include <memory>

#include "Core/Layers/Layer.hh"
#include "Editor/Panels/InspectorPanel.hh"
#include "Editor/Panels/SceneHierarchyPanel.hh"
#include "Renderer/Buffers/FrameBuffer.hh"
#include "Renderer/Buffers/IndexBuffer.hh"
#include "Renderer/Buffers/VertexBuffer.hh"
#include "Renderer/Camera/OrthographicCamera.hh"
#include "Renderer/Camera/OrthographicCameraController.hh"
#include "Renderer/Material/BaseShader.hh"
#include "Renderer/Material/Texture.hh"
#include "Scene/Scene.hh"
#include <Renderer/Camera/Camera.hh>
#include <Scene/Entity.hh>

namespace kaTe {
    class EditorLayer : public Layer {
    public:
        auto OnAttach() -> void override;
        auto OnDetach() -> void override;
        auto OnUpdate() -> void override;
        auto OnEvent(Event& event) -> void override;
        auto OnImGuiRender() -> void override;
    private:
        // ViewPort Framebuffer data
        float m_ViewPortWidth{};
        float m_ViewPortHeight{};
        bool m_VerticalSyncEnabled{};
        bool m_ViewportIsFocused{ false };
        bool m_ViewportIsHovered{ false };

        std::shared_ptr<FrameBuffer> m_FrameBuffer{};
        std::shared_ptr<BaseShader> m_ColorShader{};
        std::shared_ptr<OrthographicCameraController> m_CameraController{};
        std::shared_ptr<SceneCamera> m_MainCamera{};
        std::shared_ptr<SceneHierarchyPanel> m_HierarchyPanel{};
        std::shared_ptr<InspectorPanel> m_InspectorPanel{};

        std::shared_ptr<Scene> m_ActiveScene{};
        Entity m_SquareEntity{};
        Entity m_MainCamEntity{};

        glm::vec4 m_ClearColor{ 0.2f, 0.2f, 0.2f, 1.0f };

    };

}

#endif//KATE_ENGINE_EDITORLAYER_HH
