#ifndef KATE_ENGINE_RENDERER_API_HH
#define KATE_ENGINE_RENDERER_API_HH

#include <memory>
#include <any>

#include <glm/vec4.hpp>

#include <Tools/Common.hh>

#include <Core/Events/Event.hh>

#include <Renderer/Buffers/IndexBuffer.hh>
#include <Renderer/Buffers/VertexBuffer.hh>
#include <Renderer/Material/Shader.hh>
#include <Renderer/RenderingUtilities.hh>


namespace kaTe {
    /**
     * Describes a general Renderer interface for various graphic API e.g OpenGL
     * which is the graphics API being used right now, Vulkan, etc.
     * */
	class RendererAPI {
	public:
        explicit RendererAPI() = default;

		enum BufferBit {
			COLOR_BUFFER_BIT = 0,
			DEPTH_BUFFER_BIT = 1,
		};

        virtual auto Init() -> void = 0;
        virtual auto Shutdown() -> void = 0;

        virtual auto EnableWireframeMode() -> void = 0;
        virtual auto DisableWireframeMode() -> void = 0;

		virtual auto SetClearColor(const glm::vec4& color) -> void = 0;
		virtual auto SetClearColor(float red, float green, float blue, float alpha) -> void = 0;

        virtual auto Draw(const RenderingData& data) -> void = 0;

		virtual auto SetViewPort(UInt32_T x, UInt32_T y, UInt32_T width, UInt32_T height) -> void = 0;
        virtual auto OnEvent(Event& event) -> void = 0;

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