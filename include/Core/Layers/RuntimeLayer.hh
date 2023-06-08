/**
 * ExampleLayer.hh
 * Created by kate on 5/27/23.
 * */

#ifndef KATE_ENGINE_EXAMPLE_LAYER_HH
#define KATE_ENGINE_EXAMPLE_LAYER_HH

#include <glm/vec3.hpp>

#include <Core/Layers/Layer.hh>
#include <Core/Logger.hh>

#include "Renderer/Buffers/IndexBuffer.hh"
#include "Renderer/Buffers/VertexBuffer.hh"
#include "Renderer/OpenGL/OpenGLShader.hh"
#include <Core/EngineManager.hh>
#include <Platform/InputManager.hh>
#include <Renderer/Material/Texture2D.hh>

namespace kaTe {
    class RuntimeLayer : public Layer {
    public:
        RuntimeLayer() : Layer{ "Example" } {}

        auto onAttach() -> void override;
        auto onDetach() -> void override;
        auto onUpdate() -> void override;
        auto onEvent(Event& event) -> void override;
        auto onImGuiRender() -> void override;

    private:
        std::shared_ptr<OpenGLShader> m_Shader{};
        std::shared_ptr<OpenGLShader> m_ColorShader{};
        std::shared_ptr<OpenGLShader> m_ColorTextureShader{};
        std::shared_ptr<VertexBuffer> m_VertexBuffer{};
        std::shared_ptr<IndexBuffer> m_IndexBuffer{};

        std::shared_ptr<VertexBuffer> m_SquareVertexBuffer{};
        std::shared_ptr<IndexBuffer> m_SquareIndexBuffer{};
        std::shared_ptr<OrthographicCamera> m_Camera{};

        glm::vec3 m_CameraPosition{};
        double m_CameraMovementSpeed{ 3.0 };

        double m_CameraRotation{ };
        double m_CameraRotationSpeed{ 90.0 };

        glm::vec3 m_SquarePos{};
        double m_SquareMoveSpeed{ 3.0 };
        glm::vec3 m_SquareColor{};

        std::shared_ptr<Texture> m_Texture{};
        std::shared_ptr<Texture> m_TextureTrans{};
    };

}   // END NAMESPACE kT

#endif // KATE_ENGINE_EXAMPLE_LAYER_HH
