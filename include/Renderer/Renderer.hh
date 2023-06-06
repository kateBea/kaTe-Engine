#ifndef KATE_ENGINE_RENDERER_HH
#define KATE_ENGINE_RENDERER_HH

#include <memory>

#include <Renderer/VertexBuffer.hh>
#include <Renderer/IndexBuffer.hh>
#include <Renderer/Shader.hh>

namespace kaTe {
    class Renderer {
    public:
        enum class GraphicsAPI {
            NONE,
            OPENGL_API,
            VULKAN_API,
            COUNT,
        };

        static auto beginScene(/* With possible overloads supplying camera and other params*/) -> void;

        // Renderer::beginScene() possible overloads
        //
        // static auto beginScene(const Camera& camera) -> void;
        // static auto beginScene(const Camera& camera, const Light& light) -> void;

        static auto endScene() -> void;

        // Renderer::submit() possible overloads
        //
        // virtual auto submit(const Mesh& mesh) -> void = 0;
        static auto submit(std::shared_ptr<VertexBuffer> vertexBuffer) -> void;
        static auto submit(std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer) -> void;
        static auto submit(std::shared_ptr<Shader> shader, std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer) -> void;

        static auto flush() -> void;

        /**
		 * Returns the currently active Graphics Rendering API
		 * */
        static auto getActiveAPI() -> GraphicsAPI { return s_ActiveAPI;  }
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
        inline static GraphicsAPI s_ActiveAPI{ GraphicsAPI::OPENGL_API };

    };
}



#endif
