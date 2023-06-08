#ifndef KATE_ENGINE_RENDER_COMMAND_HH
#define KATE_ENGINE_RENDER_COMMAND_HH

#include <memory>

#include <glm/vec4.hpp>

#include "Renderer/Buffers/IndexBuffer.hh"
#include "Renderer/Buffers/VertexBuffer.hh"
#include "Renderer/OpenGL/OpenGLShader.hh"

#include <Renderer/RendererAPI.hh>
#include <Renderer/OpenGL/OpenGLRenderer.hh>

#include <Tools/Common.hh>

namespace kaTe {
	class RenderCommand {
	public:
        explicit RenderCommand() {
            // temporal
            m_ActiveRendererAPI = std::make_unique<OpenGLRenderer>();
        }

        auto init() -> void {
            m_ActiveRendererAPI->init();
        }

        auto clear(RendererAPI::BufferBit bufferBit) {
            m_ActiveRendererAPI->clear(bufferBit);
        }

        auto setClearColor(const glm::vec4& color) -> void {
            m_ActiveRendererAPI->setClearColor(color);
        }

        auto setClearColor(float red, float green, float blue, float alpha) -> void {
            m_ActiveRendererAPI->setClearColor(red, green, blue, alpha);
        }

        auto drawIndexed(std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer) -> void {
            m_ActiveRendererAPI->drawIndexed(vertexBuffer, indexBuffer);
        }

        auto drawIndexed(std::shared_ptr<OpenGLShader> shader, std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer) -> void {
            m_ActiveRendererAPI->drawIndexed(shader, vertexBuffer, indexBuffer);
        }

        auto draw(std::shared_ptr<VertexBuffer> vertexBuffer) -> void {
            m_ActiveRendererAPI->draw(vertexBuffer);
        }

        auto draw(std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer) -> void {
            m_ActiveRendererAPI->draw(vertexBuffer, indexBuffer);
        }

        auto draw(std::shared_ptr<OpenGLShader> shader, std::shared_ptr<VertexBuffer> vertexBuffer) -> void {
            m_ActiveRendererAPI->draw(shader, vertexBuffer);
        }

        auto draw(std::shared_ptr<OpenGLShader> shader, std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer) -> void {
            m_ActiveRendererAPI->draw(shader, vertexBuffer, indexBuffer);
        }

        auto refreshViewPort(UInt32_T width, UInt32_T height) -> void {
            m_ActiveRendererAPI->setViewPort(width, height);
        }

	private:
		// Non owning pointer to the currently active Graphics API
        std::unique_ptr<RendererAPI> m_ActiveRendererAPI{ nullptr };
	};
}

#endif
