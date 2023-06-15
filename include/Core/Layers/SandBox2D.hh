//
// Created by kate on 6/12/23.
//

#ifndef KATE_ENGINE_SANDBOX2D_HH
#define KATE_ENGINE_SANDBOX2D_HH

#include <memory>

#include <Core/Layers/Layer.hh>
#include <Renderer/Buffers/VertexBuffer.hh>
#include <Renderer/Buffers/IndexBuffer.hh>
#include <Renderer/Material/BaseShader.hh>
#include <Renderer/Camera/OrthographicCamera.hh>
#include <Renderer/Camera/OrthographicCameraController.hh>
#include <Renderer/Material/Texture.hh>

namespace kaTe {
    class SandBox2D : public Layer {
    public:
        auto onAttach() -> void override;
        auto onDetach() -> void override;
        auto onUpdate() -> void override;
        auto onEvent(Event& event) -> void override;
        auto onImGuiRender() -> void override;
    private:
        std::shared_ptr<BaseShader> m_ColorShader{};
        std::shared_ptr<OrthographicCameraController> m_CameraController{};
        std::shared_ptr<VertexBuffer> m_VertexBuffer{};
        std::shared_ptr<IndexBuffer> m_IndexBuffer{};

        std::shared_ptr<VertexBuffer> m_SquareVertexBuffer{};
        std::shared_ptr<IndexBuffer> m_SquareIndexBuffer{};
        std::shared_ptr<OrthographicCamera> m_Camera{};

        glm::vec3 m_SquareColor{ 0.1f, 0.5f, 0.8f };
        std::shared_ptr<Texture> m_Texture{};

        glm::vec3 m_FirstSquarePos{ -1.0f, 0.0f, -0.1 };
        double m_FirstSquareMovementSpeed{ 2.0 };

        glm::vec3 m_SecondSquarePos{ 0.0f, 0.0f, 0.0f };
        double m_SecondSquareMovementSpeed{ 1.0 };

    };

}

#endif//KATE_ENGINE_SANDBOX2D_HH
