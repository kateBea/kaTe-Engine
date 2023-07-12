//
// Created by kate on 7/1/23.
//

#ifndef KATE_ENGINE_VULKAN_TEST_LAYER_HH
#define KATE_ENGINE_VULKAN_TEST_LAYER_HH

#include "Core/Layers/Layer.hh"
#include "Renderer/Buffers/VertexBuffer.hh"
#include <Renderer/Renderer.hh>
#include <Renderer/RenderCommand.hh>

#include <Platform/InputManager.hh>

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
            RendererAPI::BufferBits bufferBits{};
            bufferBits[RendererAPI::BufferBit::COLOR_BUFFER_BIT] = true;

            RenderCommand::SetClearColor(m_ClearColor);
            //RenderCommand::Clear(bufferBits);

            Renderer::Submit(m_VertexBuffer);
        }

        auto OnEvent(Event& event) -> void override {

        }

        auto OnImGuiRender() -> void override {

        }
    private:
        std::shared_ptr<VertexBuffer> m_VertexBuffer{};

        glm::vec4 m_ClearColor{ 0.5f, 0.2f, 0.3f, 1.0f };

    };

}

#endif//KATE_ENGINE_VULKAN_TEST_LAYER_HH
