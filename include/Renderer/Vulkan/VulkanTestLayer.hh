//
// Created by kate on 7/1/23.
//

#ifndef KATE_ENGINE_VULKAN_TEST_LAYER_HH
#define KATE_ENGINE_VULKAN_TEST_LAYER_HH

#include "Core/Layers/Layer.hh"
#include "Renderer/Buffers/VertexBuffer.hh"
#include <Renderer/Renderer.hh>
#include <Renderer/RenderCommand.hh>

namespace kaTe {

    class VulkanTestLayer : public Layer {
    public:
        auto OnAttach() -> void override {
            std::vector<float> vertices{
                    0.0, -0.5, 0.0f,     1.0f, 0.0f, 0.0f, 1.0f,
                    0.5,  0.5, 0.0f,     0.0f, 1.0f, 0.0f, 1.0f,
                    -0.5,  0.5, 0.0f,     0.0f, 0.0f, 1.0f, 1.0f,
            };

            m_VertexBuffer = VertexBuffer::CreateBuffer(vertices);
        }

        auto OnDetach() -> void override {
            m_VertexBuffer->OnRelease();
        }

        auto OnUpdate() -> void override {
            RenderCommand::SetClearColor(0.5f, 0.2f, 0.3f, 1.0f);
            Renderer::Submit(m_VertexBuffer);
        }

        auto OnEvent(Event& event) -> void override {

        }

        auto OnImGuiRender() -> void override {

        }
    private:
        std::shared_ptr<VertexBuffer> m_VertexBuffer{};

    };

}

#endif//KATE_ENGINE_VULKAN_TEST_LAYER_HH
