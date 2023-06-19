#ifndef KATE_ENGINE_RENDERER_API_HH
#define KATE_ENGINE_RENDERER_API_HH

#include <memory>

#include <glm/vec4.hpp>

#include "Renderer/Buffers/IndexBuffer.hh"
#include "Renderer/Buffers/VertexBuffer.hh"
#include "Renderer/Material/BaseShader.hh"

#include <Tools/Common.hh>

namespace kaTe {
	class RendererAPI {
	public:
        explicit RendererAPI() = default;

		/* It is not an enum class for ease with bit OR operations*/
        // temporary
		enum BufferBit {
			NONE,
			OPEN_GL_COLOR_BUFFER_BIT = GL_COLOR_BUFFER_BIT,
			OPEN_GL_DEPTH_BUFFER_BIT = GL_DEPTH_BUFFER_BIT,
			COUNT,
		};

        virtual auto Init() -> void = 0;
        virtual auto Shutdown() -> void = 0;
		virtual auto SetClearColor(const glm::vec4& color) -> void = 0;
		virtual auto SetClearColor(float red, float green, float blue, float alpha) -> void = 0;
		virtual auto Clear(BufferBit bufferBits) -> void = 0;

		virtual auto DrawIndexed(std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer) -> void = 0;
        virtual auto DrawIndexed(std::shared_ptr<BaseShader> shader, std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer) -> void = 0;

        virtual auto Draw(std::shared_ptr<VertexBuffer> vertexBuffer) -> void = 0;
		virtual auto Draw(std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer) -> void = 0;

		virtual auto Draw(std::shared_ptr<BaseShader> shader, std::shared_ptr<VertexBuffer> vertexBuffer) -> void = 0;
        virtual auto Draw(std::shared_ptr<BaseShader> shader, std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer) -> void = 0;

		virtual auto SetViewPort(UInt32_T x, UInt32_T y, UInt32_T width, UInt32_T height) -> void = 0;

        virtual ~RendererAPI() = default;
    public:
        // Forbidden operations
        RendererAPI(const RendererAPI&) = delete;
        auto operator=(const RendererAPI&) -> RendererAPI& = delete;

        RendererAPI(RendererAPI&&) = delete;
        auto operator=(RendererAPI&&) -> RendererAPI& = delete;
	};
}

#endif