#ifndef KATE_ENGINE_RENDER_COMMAND_HH
#define KATE_ENGINE_RENDER_COMMAND_HH

#include <memory>

#include <glm/vec4.hpp>

#include "Renderer/Buffers/IndexBuffer.hh"
#include "Renderer/Buffers/VertexBuffer.hh"

#include <Renderer/Renderer.hh>
#include <Renderer/RendererAPI.hh>
#include <Renderer/Material/BaseShader.hh>

#include <Core/Logger.hh>
#include <Tools/Common.hh>

namespace kaTe {
	class RenderCommand {
	public:
        static auto init() -> void;
        static auto shutDown() -> void;

        static auto clear(RendererAPI::BufferBit bufferBit) -> void;
        static auto setClearColor(const glm::vec4& color) -> void;
        static auto setClearColor(float red, float green, float blue, float alpha) -> void;

        static auto drawIndexed(std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer) -> void;
        static auto drawIndexed(std::shared_ptr<BaseShader> shader, std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer) -> void;

        static auto draw(std::shared_ptr<VertexBuffer> vertexBuffer) -> void;
        static auto draw(std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer) -> void;
        static auto draw(std::shared_ptr<BaseShader> shader, std::shared_ptr<VertexBuffer> vertexBuffer) -> void;
        static auto draw(std::shared_ptr<BaseShader> shader, std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer) -> void;

        static auto refreshViewPort(UInt32_T width, UInt32_T height) -> void;

	private:
        inline static RendererAPI* s_ActiveRendererAPI{ nullptr };
	};
}

#endif
