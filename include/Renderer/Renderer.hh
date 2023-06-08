#ifndef KATE_ENGINE_RENDERER_HH
#define KATE_ENGINE_RENDERER_HH

#include <memory>

#include <glm/mat4x4.hpp>

#include "Renderer/Buffers/IndexBuffer.hh"
#include "Renderer/Buffers/VertexBuffer.hh"
#include "Renderer/Camera/OrthographicCamera.hh"
#include "Renderer/OpenGL/OpenGLShader.hh"
#include <Renderer/RenderCommand.hh>

namespace kaTe {
    class Renderer {
    public:
        explicit Renderer() = default;
        virtual ~Renderer() = default;
        enum class GraphicsAPI {
            NONE,
            OPENGL_API,
            VULKAN_API,
            COUNT,
        };

        virtual auto init() -> void;

        auto beginScene(std::shared_ptr<OrthographicCamera> camera) -> void;

        // Renderer::beginScene() possible overloads
        //
        // static auto beginScene(const Camera& camera) -> void;
        // static auto beginScene(const Camera& camera, const Light& light) -> void;

        auto endScene() -> void;

        // Renderer::submit() possible overloads
        //
        // virtual auto submit(const Mesh& mesh) -> void = 0;
        auto submit(std::shared_ptr<VertexBuffer> vertexBuffer) -> void;
        auto submit(std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer) -> void;
        auto submit(std::shared_ptr<OpenGLShader> shader, std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer, const glm::mat4 &transform = glm::mat4(1.0)) -> void;

        auto getRenderCommand() -> RenderCommand& { return m_RenderCommand; }

        auto flush() -> void;

        /**
		 * Returns the currently active Graphics Rendering API
		 * */
        auto getActiveAPI() -> GraphicsAPI { return m_ActiveAPI;  }
    private:
        // Forbidden operations
        Renderer(const Renderer&) = delete;
        auto operator=(const Renderer&) -> Renderer& = delete;

        Renderer(Renderer&&) = delete;
        auto operator=(Renderer&&) -> Renderer& = delete;

    private:
        // States the active Graphics Rendering API for the current window.
        // For the time being, we only have one main window, therefore, this attribute is going
        // to be static. In case we want to try different API at runtime, we may
        // have more than one Renderer API specific active
        GraphicsAPI m_ActiveAPI{ GraphicsAPI::OPENGL_API };
    private:
        struct SceneData {
            std::shared_ptr<OrthographicCamera> camera{};
        };

        SceneData m_SceneData{ .camera{} };
        RenderCommand m_RenderCommand{};
    };
}



#endif
