//
// Created by kate on 6/12/23.
//

#include <utility>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Core/Assert.hh>

#include "Renderer/Renderer2D.hh"
#include <Renderer/RenderCommand.hh>
#include "Renderer/OpenGL/OpenGLShader.hh"

namespace kaTe {

    auto Renderer2D::Init() -> void {
        s_DrawData          = std::make_unique<Renderer2DDrawData>();
        s_RenderingStats    = std::make_unique<Renderer2DStats>();
        KT_ASSERT(s_DrawData, "Renderer 2D draw data pointer is NULL");
        KT_ASSERT(s_RenderingStats, "Renderer 2D stats pointer is NULL");

        std::vector<float> squareData {
                // Positions            // Texture coordinates
                -0.5f,  -0.5f, 0.0f,     0.0f, 0.0f,   // bottom left
                 0.5f,  -0.5f, 0.0f,     1.0f, 0.0f,   // bottom right
                 0.5f,   0.5f, 0.0f,     1.0f, 1.0f,   // top right
                -0.5f,   0.5f, 0.0f,     0.0f, 1.0f,   // top left
        };

        s_DrawData->vertexBuffer    = VertexBuffer::CreateBuffer(squareData);
        s_DrawData->indexBuffer     = IndexBuffer::CreateBuffer({0, 1, 2, 2, 3, 0});
        s_DrawData->colorShader     = BaseShader::CreateShader("../assets/shaders/debugShaderVert.glsl", "../assets/shaders/colorShader.glsl");
        s_DrawData->textureShader   = BaseShader::CreateShader("../assets/shaders/textureVert.glsl", "../assets/shaders/textureFrag.glsl");

        s_DrawData->textureShader->SetInt("u_TextSampler", 0);
        s_DrawData->vertexBuffer->SetBufferLayout(BufferLayout{{ ShaderDataType::FLOAT3_TYPE, "a_Position" }, { ShaderDataType::FLOAT2_TYPE, "a_TextureCoordinates" }});
    }

    auto Renderer2D::BeginScene(std::shared_ptr<OrthographicCamera> camera) -> void {
        s_DrawData->orthographicCamera = std::move(camera);
    }

    auto Renderer2D::BeginScene(std::shared_ptr<Camera> camera) -> void {
        s_DrawData->camera = std::move(camera);
    }

    auto Renderer2D::EndScene() -> void {
        s_SavedSceneStats = std::make_unique<Renderer2DStats>(*s_RenderingStats);
        s_RenderingStats->Reset();
    }

    auto Renderer2D::DrawQuad(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color, double angle, bool useOrthoCamera) -> void {
        DrawQuad(glm::vec3(position, 0.0), size, color, angle, useOrthoCamera);
    }

    auto Renderer2D::DrawQuad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color, double angle, bool useOrthoCamera) -> void {
        glm::mat4 cameraViewProj{};

        if (!useOrthoCamera)
            cameraViewProj = s_DrawData->camera->GetProjection() * s_DrawData->camera->GetTransform();
        else
            cameraViewProj = s_DrawData->orthographicCamera->GetProjectionView();

        // Data Setup
        static constexpr glm::vec3 ZAxis{ 0.0f, 0.0f, 1.0f };
        static constexpr glm::mat4 IdentityMatrix{ glm::mat4(1.0) };

        glm::mat4 scale{ glm::scale(IdentityMatrix, glm::vec3(size, 1.0f)) };
        glm::mat4 rotation{ glm::rotate(IdentityMatrix, (float)glm::radians(angle), ZAxis) };
        glm::mat4 transform{ glm::translate(IdentityMatrix, position) * scale * rotation };

        s_DrawData->colorShader->SetVec4("u_Color", color);
        s_DrawData->colorShader->SetMat4("u_ProjectionView", cameraViewProj);

        DrawQuad(transform, color, useOrthoCamera);
    }

    auto Renderer2D::ShutDown() -> void {

    }

    auto Renderer2D::DrawQuad(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color, double angle, std::shared_ptr<Texture> texture, bool useOrthoCamera) -> void {
        DrawQuad(glm::vec3(position, 0.0), size, color, angle, std::move(texture), useOrthoCamera);
    }

    auto Renderer2D::DrawQuad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color, double angle, std::shared_ptr<Texture> texture, bool useOrthoCamera) -> void {
        // Data Setup
        static constexpr glm::vec3 zAxis{ 0.0f, 0.0f, 1.0f };
        static constexpr glm::mat4 identMat{ glm::mat4(1.0) };

        glm::mat4 scale{ glm::scale(identMat, glm::vec3(size, 1.0f)) };
        glm::mat4 rotation{ glm::rotate(identMat, (float)glm::radians(angle), zAxis) };
        glm::mat4 transform{ glm::translate(identMat, position) * scale * rotation };

        DrawQuad(transform, texture, useOrthoCamera);
    }

    auto Renderer2D::DrawQuad(const glm::mat4 &transform, const glm::vec4 &color, bool useOrthoCamera) -> void {
        glm::mat4 cameraViewProj{};

        if (!useOrthoCamera)
            cameraViewProj = s_DrawData->camera->GetProjection() * s_DrawData->camera->GetTransform();
        else
            cameraViewProj = s_DrawData->orthographicCamera->GetProjectionView();

        s_DrawData->colorShader->SetVec4("u_Color", color);
        s_DrawData->colorShader->SetMat4("u_ProjectionView", cameraViewProj);
        s_DrawData->colorShader->SetMat4("u_Transform", transform);

        // Render
        RenderCommand::DrawIndexed(s_DrawData->colorShader, s_DrawData->vertexBuffer, s_DrawData->indexBuffer);

        // Rendering Stats management
        s_RenderingStats->IncrementQuadCount(1);
        // We increment the number of draw calls because for now
        // The RenderCommand directly flushes the draw call
        s_RenderingStats->IncrementDrawCallCount(1);
    }

    auto Renderer2D::DrawQuad(const glm::mat4 &transform, std::shared_ptr<Texture> texture, bool useOrthoCamera) -> void {
        glm::mat4 cameraViewProj{};

        if (!useOrthoCamera)
            cameraViewProj = s_DrawData->camera->GetProjection() * s_DrawData->camera->GetTransform();
        else
            cameraViewProj = s_DrawData->orthographicCamera->GetProjectionView();

        s_DrawData->textureShader->SetMat4("u_ProjectionView", cameraViewProj);
        s_DrawData->textureShader->SetMat4("u_Transform", transform);
        texture->Bind();

        // Render
        RenderCommand::DrawIndexed(s_DrawData->textureShader, s_DrawData->vertexBuffer, s_DrawData->indexBuffer);

        // Rendering Stats management
        s_RenderingStats->IncrementQuadCount(1);
        // We increment the number of draw calls because for now
        // The RenderCommand directly flushes the draw call
        s_RenderingStats->IncrementDrawCallCount(1);
    }
}