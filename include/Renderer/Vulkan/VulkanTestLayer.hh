//
// Created by kate on 7/1/23.
//

#ifndef KATE_ENGINE_VULKAN_TEST_LAYER_HH
#define KATE_ENGINE_VULKAN_TEST_LAYER_HH

#include "Core/Layer.hh"
#include "Renderer/Buffers/VertexBuffer.hh"
#include <Renderer/RenderCommand.hh>
#include <Renderer/Renderer.hh>

#include <Platform/InputManager.hh>

namespace kaTe {

    class VulkanTestLayer : public Layer {
    public:
        auto OnAttach() -> void override {

        }

        auto OnDetach() -> void override {
            m_VertexBuffer->OnRelease();
        }

        auto OnUpdate() -> void override {

            RenderCommand::SetClearColor(m_ClearColor);

            //Renderer::Submit(<#initializer #>);
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
