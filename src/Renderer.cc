//
// Created by kate on 6/5/23.
//

#include <Core/Assert.hh>

#include <Renderer/Renderer.hh>
#include <Renderer/RenderCommand.hh>
#include <Renderer/OpenGL/OpenGLShader.hh>
#include <utility>

namespace kaTe {
    auto Renderer::BeginScene(std::shared_ptr<OrthographicCamera> camera) -> void {
        s_SceneData->camera = std::move(camera);
    }

    auto Renderer::Submit(std::shared_ptr<VertexBuffer> vertexBuffer) -> void {

    }

    auto Renderer::Submit(std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer) -> void {
        RenderCommand::DrawIndexed(std::move(vertexBuffer), std::move(indexBuffer));
    }

    auto Renderer::Submit(std::shared_ptr<BaseShader> shader, std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer, const glm::mat4 &transform) -> void {
        // Currently submit draws our geometry directly, not buffered
        shader->SetMat4("u_ProjectionView", s_SceneData->camera->GetProjectionView());
        shader->SetMat4("u_Transform", transform);
        RenderCommand::DrawIndexed(shader, std::move(vertexBuffer), std::move(indexBuffer));
    }

    auto Renderer::EndScene() -> void {

    }

    auto Renderer::Flush() -> void {

    }

    auto Renderer::Init() -> void {
        s_SceneData = new (std::nothrow) SceneData();
        KT_ASSERT(s_SceneData, "Renderer Scene data pointer is NULL");
    }

    auto Renderer::ShutDown() -> void {
        delete s_SceneData;
    }

    auto Renderer::Submit(const DrawData& data) -> void {

    }

    auto Renderer::OnWindowResize(UInt32_T x, UInt32_T y, UInt32_T width, UInt32_T height) -> void {
        // Temporary. Should change when we have multiple frame buffers to render to
        RenderCommand::UpdateViewPort(x, y, width, height);
    }
}
