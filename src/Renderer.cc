//
// Created by kate on 6/5/23.
//

#include <Renderer/Renderer.hh>

namespace kaTe {
    auto Renderer::beginScene(std::shared_ptr<OrthographicCamera> camera) -> void {
        m_SceneData.camera = camera;
    }

    auto Renderer::submit(std::shared_ptr<VertexBuffer> vertexBuffer) -> void {

    }

    auto Renderer::submit(std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer) -> void {
        m_RenderCommand.drawIndexed(vertexBuffer, indexBuffer);
    }

    auto Renderer::submit(std::shared_ptr<Shader> shader, std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer) -> void {
        shader->setUniformMat4("u_ProjectionView", m_SceneData.camera->getProjectionView());
        m_RenderCommand.drawIndexed(shader, vertexBuffer, indexBuffer);
    }

    auto Renderer::endScene() -> void {

    }

    auto Renderer::flush() -> void {

    }
}
