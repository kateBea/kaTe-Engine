//
// Created by kate on 6/12/23.
//

#ifndef KATE_ENGINE_RENDERER2D_HH
#define KATE_ENGINE_RENDERER2D_HH

#include <memory>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <Renderer/Renderer.hh>
#include <Renderer/Camera/OrthographicCamera.hh>
#include <Renderer/Material/BaseShader.hh>

namespace kaTe {
    class Renderer2D {
    public:
        static auto Init() -> void;
        static auto ShutDown() -> void;

        static auto BeginScene(std::shared_ptr<OrthographicCamera> camera) -> void;
        static auto EndScene() -> void;

        // NOTE: the angle is in degrees
        static auto DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, double angle) -> void;
        static auto DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, double angle) -> void;

        static auto DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, double angle, std::shared_ptr<Texture> texture) -> void;
        static auto DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, double angle, std::shared_ptr<Texture> texture) -> void;

    private:
        struct Renderer2DDrawData {
            std::shared_ptr<VertexBuffer> vertexBuffer{};
            std::shared_ptr<IndexBuffer> indexBuffer{};
            std::shared_ptr<BaseShader> colorShader{};
            std::shared_ptr<BaseShader> textureShader{};
            std::shared_ptr<OrthographicCamera> camera{};
        };


        inline static std::unique_ptr<Renderer2DDrawData> s_DrawData{};

    };
}


#endif//KATE_ENGINE_RENDERER2D_HH
