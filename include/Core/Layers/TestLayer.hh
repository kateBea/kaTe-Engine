/**
 * ExampleLayer.hh
 * Created by kate on 5/27/23.
 * */

#ifndef KATE_ENGINE_EXAMPLE_LAYER_HH
#define KATE_ENGINE_EXAMPLE_LAYER_HH

#include "glm/vec3.hpp"

#include "Core/Layers/Layer.hh"
#include "Core/Logger.hh"

#include "Core/Application.hh"
#include "Platform/InputManager.hh"
#include "Renderer/Buffers/IndexBuffer.hh"
#include "Renderer/Buffers/VertexBuffer.hh"
#include "Renderer/Camera/OrthographicCamera.hh"
#include "Renderer/Camera/OrthographicCameraController.hh"
#include "Renderer/Material/Texture2D.hh"
#include "Renderer/OpenGL/OpenGLShader.hh"

namespace kaTe {
    class TestLayer : public Layer {
    public:
        TestLayer() : Layer{ "Test Layer" } {}

        auto OnAttach() -> void override;
        auto OnDetach() -> void override;
        auto OnUpdate() -> void override;
        auto OnEvent(Event& event) -> void override;
        auto OnImGuiRender() -> void override;

    private:
        std::shared_ptr<BaseShader> m_ColorShader{};
        std::shared_ptr<BaseShader> m_ColorTextureShader{};
        std::shared_ptr<VertexBuffer> m_VertexBuffer{};
        std::shared_ptr<IndexBuffer> m_IndexBuffer{};

        std::shared_ptr<VertexBuffer> m_SquareVertexBuffer{};
        std::shared_ptr<IndexBuffer> m_SquareIndexBuffer{};
        std::shared_ptr<OrthographicCameraController> m_CameraController{};

        glm::vec3 m_CameraPosition{};
        double m_CameraMovementSpeed{ 3.0 };

        glm::vec3 m_SquarePos{};
        glm::vec3 m_SquareColor{};

        std::shared_ptr<Texture> m_Texture{};
        std::shared_ptr<Texture> m_TextureTrans{};
    };

}   // END NAMESPACE kT

#endif // KATE_ENGINE_EXAMPLE_LAYER_HH
