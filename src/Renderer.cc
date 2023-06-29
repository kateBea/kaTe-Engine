/**
 * Renderer.cc
 * Created by kate on 6/5/23.
 * */

// C++ Standard Library
#include <new>
#include <memory>
#include <utility>

// Third-Party Libraries
#include <glm/glm.hpp>

// Project Headers
#include <Tools/Common.hh>
#include <Core/Assert.hh>

#include <Renderer/Renderer.hh>
#include <Renderer/RenderCommand.hh>

namespace kaTe {
    auto Renderer::BeginScene(std::shared_ptr<OrthographicCamera> camera) -> void {
        s_DrawData->OrthographicCameraForScene = std::move(camera);
    }

    auto Renderer::BeginScene(std::shared_ptr<Camera> camera) -> void {
        s_DrawData->CameraForScene = std::move(camera);
    }

    auto Renderer::Submit(const std::shared_ptr<VertexBuffer> &vertexBuffer) -> void {
        RenderCommand::Draw(vertexBuffer);
    }

    auto Renderer::Submit(std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer) -> void {
        RenderCommand::DrawIndexed(std::move(vertexBuffer), std::move(indexBuffer));
    }

    auto Renderer::Submit(const std::shared_ptr<BaseShader>& shader, std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer, const glm::mat4 &transform) -> void {
        // Currently submit draws our geometry directly, not buffered
        shader->SetMat4("u_ProjectionView", s_DrawData->OrthographicCameraForScene->GetProjectionView());
        shader->SetMat4("u_Transform", transform);
        RenderCommand::DrawIndexed(shader, std::move(vertexBuffer), std::move(indexBuffer));
    }

    auto Renderer::EndScene() -> void {
        s_SavedSceneStats = std::make_unique<RenderingStats>(*s_RenderingStats);
        s_RenderingStats->Reset();
    }

    auto Renderer::Flush() -> void {

    }

    auto Renderer::Init() -> void {
        s_DrawData = std::make_unique<RendererDrawData>();
        s_RenderingStats    = std::make_unique<RenderingStats>();
        KT_ASSERT(s_DrawData, "Renderer draw data pointer is NULL");
        KT_ASSERT(s_RenderingStats, "Renderer stats pointer is NULL");
    }

    auto Renderer::ShutDown() -> void {

    }

    [[maybe_unused]] auto Renderer::OnWindowResize(UInt32_T x, UInt32_T y, UInt32_T width, UInt32_T height) -> void {
        // Temporary. Should change when we have multiple frame buffers to render to
        RenderCommand::UpdateViewPort(x, y, width, height);
    }

    auto Renderer::SubmitQuad(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color, double angle, bool useOrthographicCamera) -> void {

    }

    auto Renderer::SubmitQuad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color, double angle, bool useOrthographicCamera) -> void {

    }

    auto Renderer::SubmitQuad(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color, double angle, const std::shared_ptr<Texture> &texture, bool useOrthoCamera) -> void {

    }

    auto Renderer::SubmitQuad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color, double angle, const std::shared_ptr<Texture> &texture, bool useOrthographicCamera) -> void {

    }

    auto Renderer::SubmitQuad(const glm::mat4 &transform, const glm::vec4 &color, bool useOrthographicCamera) -> void {

    }

    auto Renderer::SubmitQuad(const glm::mat4 &transform, const std::shared_ptr<Texture> &texture, bool useOrthographicCamera) -> void {

    }

}
