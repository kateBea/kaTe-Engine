//
// Created by kate on 6/12/23.
//

#ifndef KATE_ENGINE_EDITOR_LAYER_HH
#define KATE_ENGINE_EDITOR_LAYER_HH

#include <memory>

#include "Core/Layers/Layer.hh"
#include "Renderer/Buffers/FrameBuffer.hh"
#include "Renderer/Buffers/IndexBuffer.hh"
#include "Renderer/Buffers/VertexBuffer.hh"
#include "Renderer/Camera/OrthographicCamera.hh"
#include "Renderer/Camera/OrthographicCameraController.hh"
#include "Renderer/Material/BaseShader.hh"
#include "Renderer/Material/Texture.hh"
#include "Scene/Scene.hh"
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
        bool m_VerticalSyncEnabled{};
        bool m_ViewportIsFocused{ false };
        bool m_ViewportIsHovered{ false };
        std::shared_ptr<FrameBuffer> m_FrameBuffer{};
        std::shared_ptr<BaseShader> m_ColorShader{};
        std::shared_ptr<OrthographicCameraController> m_CameraController{};
        std::shared_ptr<VertexBuffer> m_VertexBuffer{};
        std::shared_ptr<IndexBuffer> m_IndexBuffer{};

        std::shared_ptr<VertexBuffer> m_SquareVertexBuffer{};
        std::shared_ptr<IndexBuffer> m_SquareIndexBuffer{};
        std::shared_ptr<OrthographicCamera> m_Camera{};

        std::shared_ptr<Scene> m_ActiveScene{};
        Entity m_Square{};

        glm::vec3 m_SquareColor{ 0.1f, 0.5f, 0.8f };
        glm::vec4 m_ClearColor{ 0.2f, 0.2f, 0.2f, 1.0f };

        // ViewPort Framebuffer data
        float m_ViewPortWidth{};
        float m_ViewPortHeight{};

    };

}

#endif//KATE_ENGINE_EDITORLAYER_HH
