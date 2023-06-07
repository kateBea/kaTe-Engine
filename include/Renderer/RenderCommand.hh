#ifndef KATE_ENGINE_RENDER_COMMAND_HH
#define KATE_ENGINE_RENDER_COMMAND_HH

#include <glm/vec4.hpp>

#include <Renderer/RendererAPI.hh>

#include <Renderer/Renderer.hh>
#include <Renderer/VertexBuffer.hh>
#include <Renderer/IndexBuffer.hh>

// Temporal
#include <Renderer/RendererAPI.hh>
#include <Renderer/OpenGL/OpenGLRenderer.hh>

#include <Tools/Common.hh>

namespace kaTe {
	class RenderCommand {
	public:
        using BufferBit = RendererAPI::BufferBit;

        explicit RenderCommand() {
            // temporal
            s_ActiveRendererAPI = new OpenGLRenderer();
        }

        static auto clear(BufferBit bufferBit) {
            s_ActiveRendererAPI->clear(bufferBit);
        }

        static auto setClearColor(const glm::vec4& color) -> void {
            s_ActiveRendererAPI->setClearColor(color);
        }

        static auto setClearColor(float red, float green, float blue, float alpha) -> void {
            s_ActiveRendererAPI->setClearColor(red, green, blue, alpha);
        }

        static auto drawIndexed(std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer) -> void {
            s_ActiveRendererAPI->drawIndexed(vertexBuffer, indexBuffer);
        }

        static auto drawIndexed(std::shared_ptr<Shader> shader, std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer) -> void {
            s_ActiveRendererAPI->drawIndexed(shader, vertexBuffer, indexBuffer);
        }

        static auto draw(std::shared_ptr<VertexBuffer> vertexBuffer) -> void {
            s_ActiveRendererAPI->draw(vertexBuffer);
        }

        static auto draw(std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer) -> void {
            s_ActiveRendererAPI->draw(vertexBuffer, indexBuffer);
        }

        static auto draw(std::shared_ptr<Shader> shader, std::shared_ptr<VertexBuffer> vertexBuffer) -> void {
            s_ActiveRendererAPI->draw(shader, vertexBuffer);
        }

        static auto draw(std::shared_ptr<Shader> shader, std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer) -> void {
            s_ActiveRendererAPI->draw(shader, vertexBuffer, indexBuffer);
        }

        static auto refreshViewPort(UInt32_T width, UInt32_T height) -> void {
            s_ActiveRendererAPI->setViewPort(width, height);
        }

	private:
		// Non owning pointer to the currently active Graphics API
		inline static RendererAPI* s_ActiveRendererAPI{ nullptr };
	};
}

#endif
