/**
 * ExampleLayer.hh
 * Created by kate on 5/27/23.
 * */

#ifndef KATE_ENGINE_EXAMPLE_LAYER_HH
#define KATE_ENGINE_EXAMPLE_LAYER_HH

#include <Core/Layers/Layer.hh>
#include <Core/Logger.hh>

#include <Renderer/Shader.hh>
#include <Renderer/VertexBuffer.hh>
#include <Renderer/IndexBuffer.hh>
#include <Core/EngineManager.hh>
#include <Platform/InputManager.hh>

namespace kaTe {
    class TestLayer : public Layer {
    public:
        TestLayer() : Layer{ "Example" } {}

        auto onAttach() -> void override;
        auto onDetach() -> void override;
        auto onUpdate() -> void override;
        auto onEvent(Event& event) -> void override;

    private:
        std::shared_ptr<Shader> m_Shader{};
        std::shared_ptr<VertexBuffer> m_VertexBuffer{};
        std::shared_ptr<IndexBuffer> m_IndexBuffer{};

        std::shared_ptr<VertexBuffer> m_SquareVertexBuffer{};
        std::shared_ptr<IndexBuffer> m_SquareIndexBuffer{};
        std::shared_ptr<OrthographicCamera> m_Camera{};

        glm::vec3 m_CameraPosition{};
        double m_CameraMovementSpeed{ 0.5 };

        double m_CameraRotation{ };
        double m_CameraRotationSpeed{ 0.1 };
    };

}   // END NAMESPACE kT

#endif // KATE_ENGINE_EXAMPLE_LAYER_HH
