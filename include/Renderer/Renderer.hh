#ifndef KATE_ENGINE_RENDERER_HH
#define KATE_ENGINE_RENDERER_HH

#include <memory>

#include <glm/mat4x4.hpp>

#include <Renderer/Material/BaseShader.hh>
#include "Renderer/Buffers/IndexBuffer.hh"
#include "Renderer/Buffers/VertexBuffer.hh"
#include "Renderer/Camera/OrthographicCamera.hh"

#include <Renderer/Material/Texture.hh>

#include <Tools/Common.hh>

namespace kaTe {
    struct DrawData {
        std::shared_ptr<BaseShader> shader{};
        std::shared_ptr<VertexBuffer> vertexBuffer{};
        std::shared_ptr<IndexBuffer> indexBuffer{};
        std::vector<std::shared_ptr<Texture>> textures{};
        glm::mat4 transform{ glm::mat4(1.0) };
    };

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

        static auto Init() -> void;

        static auto BeginScene(std::shared_ptr<OrthographicCamera> camera) -> void;
        static auto EndScene() -> void;

        static auto Submit(std::shared_ptr<VertexBuffer> vertexBuffer) -> void;
        static auto Submit(std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer) -> void;
        static auto Submit(std::shared_ptr<BaseShader> shader, std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer, const glm::mat4 &transform = glm::mat4(1.0)) -> void;

        static auto Submit(const DrawData& data) -> void;
        static auto Flush() -> void;

        static auto ShutDown() -> void;

        static auto OnWindowResize(UInt32_T x, UInt32_T y, UInt32_T width, UInt32_T height) -> void;

        static auto GetActiveGraphicsAPI() -> GraphicsAPI { return s_ActiveAPI;  }
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
    private:
        struct SceneData {
            std::shared_ptr<OrthographicCamera> camera{};
        };

        inline static SceneData* s_SceneData{};
    };
}



#endif
