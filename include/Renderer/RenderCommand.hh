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
        using BufferBits = RendererAPI::BufferBits;
        static auto Init() -> void;
        static auto ShutDown() -> void;

        static auto EnableWireframeMode() -> void;
        static auto DisableWireframeMode() -> void;

        static auto Clear(BufferBits bufferBits) -> void;
        static auto SetClearColor(const glm::vec4& color) -> void;
        static auto SetClearColor(float red, float green, float blue, float alpha) -> void;

        static auto DrawIndexed(const std::shared_ptr<VertexBuffer>& vertexBuffer, const std::shared_ptr<IndexBuffer>& indexBuffer) -> void;
        static auto DrawIndexed(const std::shared_ptr<BaseShader>& shader, const std::shared_ptr<VertexBuffer>& vertexBuffer, const std::shared_ptr<IndexBuffer>& indexBuffer) -> void;

        static auto Draw(const std::shared_ptr<VertexBuffer>& vertexBuffer) -> void;
        static auto Draw(const std::shared_ptr<VertexBuffer>& vertexBuffer, const std::shared_ptr<IndexBuffer>& indexBuffer) -> void;
        static auto Draw(const std::shared_ptr<BaseShader>& shader, const std::shared_ptr<VertexBuffer>& vertexBuffer) -> void;
        static auto Draw(const std::shared_ptr<BaseShader>& shader, const std::shared_ptr<VertexBuffer>& vertexBuffer, const std::shared_ptr<IndexBuffer>& indexBuffer) -> void;

        static auto UpdateViewPort(UInt32_T x, UInt32_T y, UInt32_T width, UInt32_T height) -> void;

	private:
        inline static RendererAPI* s_ActiveRendererAPI{ nullptr };
	};
}

#endif
