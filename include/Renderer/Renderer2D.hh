//
// Created by kate on 6/12/23.
//

#ifndef KATE_ENGINE_RENDERER2D_HH
#define KATE_ENGINE_RENDERER2D_HH

#include <memory>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <Tools/Common.hh>
#include <Renderer/Renderer.hh>
#include <Renderer/Camera/OrthographicCamera.hh>
#include <Renderer/Camera/Camera.hh>
#include <Renderer/Material/BaseShader.hh>

namespace kaTe {
    class Renderer2D {
    public:
        static auto Init() -> void;
        static auto ShutDown() -> void;

        static auto BeginScene(std::shared_ptr<Camera> camera) -> void;
        static auto BeginScene(std::shared_ptr<OrthographicCamera> camera) -> void;

        static auto EndScene() -> void;

        // NOTE: the angle is in degrees
        static auto DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, double angle, bool useOrthoCamera = false) -> void;
        static auto DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, double angle, bool useOrthoCamera = false) -> void;

        static auto DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, double angle, std::shared_ptr<Texture> texture, bool useOrthoCamera = false) -> void;
        static auto DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, double angle, std::shared_ptr<Texture> texture, bool useOrthoCamera = false) -> void;

        static auto DrawQuad(const glm::mat4& transform, const glm::vec4& color, bool useOrthoCamera = false) -> void;
        static auto DrawQuad(const glm::mat4& transform, const std::shared_ptr<Texture>& texture,  bool useOrthoCamera = false) -> void;

        KT_NODISCARD static auto QueryDrawCallsCount() -> UInt32_T { return s_SavedSceneStats->GetDrawCallsCount(); }
        KT_NODISCARD static auto QueryQuadCount() -> UInt32_T { return s_SavedSceneStats->GetQuadCount(); }
        KT_NODISCARD static auto QueryIndexCount() -> UInt32_T { return s_SavedSceneStats->GetIndexCount(); }
        KT_NODISCARD static auto QueryVertexCount() -> UInt32_T { return s_SavedSceneStats->GetVertexCount(); }
    private:
        struct Renderer2DStats {
            explicit Renderer2DStats() = default;
            Renderer2DStats(const Renderer2DStats& other) = default;

            KT_NODISCARD auto GetQuadCount() const -> UInt32_T { return m_QuadCount; }
            KT_NODISCARD auto GetDrawCallsCount() const -> UInt32_T { return m_DrawCallsCount; }

            // This value is fixed since all quads need 4 vertices at most
            KT_NODISCARD auto GetVertexCount() const -> UInt32_T { return m_QuadCount * 4; }
            // This value is fixed since all quads need 6 indices at most
            KT_NODISCARD auto GetIndexCount() const -> UInt32_T { return m_QuadCount * 6; }

            auto IncrementDrawCallCount(UInt32_T value) { m_DrawCallsCount += value; }
            auto IncrementQuadCount(UInt32_T value) { m_QuadCount += value; }

            auto Reset() -> void {
                m_DrawCallsCount = 0;
                m_QuadCount = 0;
            }

            UInt32_T m_DrawCallsCount{};
            UInt32_T m_QuadCount{};
        };

        struct Renderer2DDrawData {
            std::shared_ptr<VertexBuffer> vertexBuffer{};
            std::shared_ptr<IndexBuffer> indexBuffer{};
            std::shared_ptr<BaseShader> colorShader{};
            std::shared_ptr<BaseShader> textureShader{};
            std::shared_ptr<OrthographicCamera> orthographicCamera{};
            std::shared_ptr<Camera> camera{};
        };


        inline static std::unique_ptr<Renderer2DDrawData>   s_DrawData{};

        // TODO: implement thread safety
        inline static std::unique_ptr<Renderer2DStats>      s_RenderingStats{};
        inline static std::unique_ptr<Renderer2DStats>      s_SavedSceneStats{};
    };
}


#endif//KATE_ENGINE_RENDERER2D_HH
