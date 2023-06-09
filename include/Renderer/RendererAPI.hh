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
		enum BufferBit {
			NONE,
			OPEN_GL_COLOR_BUFFER_BIT = GL_COLOR_BUFFER_BIT,
			OPEN_GL_DEPTH_BUFFER_BIT = GL_DEPTH_BUFFER_BIT,
			COUNT,
		};

        virtual auto init() -> void = 0;
		virtual auto setClearColor(const glm::vec4& color) -> void = 0;
		virtual auto setClearColor(float red, float green, float blue, float alpha) -> void = 0;
		virtual auto clear(BufferBit bufferBits) -> void = 0;

		virtual auto drawIndexed(std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer) -> void = 0;
        virtual auto drawIndexed(std::shared_ptr<BaseShader> shader, std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer) -> void = 0;

        // RendererAPI::draw() possible overloads
        //
        // virtual auto draw(const Mesh& mesh) -> void = 0;
        virtual auto draw(std::shared_ptr<VertexBuffer> vertexBuffer) -> void = 0;
		virtual auto draw(std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer) -> void = 0;
		// There's no reason to draw a mesh indexed as it may probably have its own indices
	
		// virtual auto draw(const Shader& shader, const Mesh& mesh) -> void = 0;
		virtual auto draw(std::shared_ptr<BaseShader> shader, std::shared_ptr<VertexBuffer> vertexBuffer) -> void = 0;
        virtual auto draw(std::shared_ptr<BaseShader> shader, std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer) -> void = 0;

		virtual auto setViewPort(UInt32_T width, UInt32_T height) -> void = 0;

        virtual ~RendererAPI() = default;
    private:
        // Forbidden operations
        RendererAPI(const RendererAPI&) = delete;
        auto operator=(const RendererAPI&) -> RendererAPI& = delete;

        RendererAPI(RendererAPI&&) = delete;
        auto operator=(RendererAPI&&) -> RendererAPI& = delete;
	};
}

#endif