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

    auto Renderer::submit(std::shared_ptr<OpenGLShader> shader, std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer, const glm::mat4 &transform) -> void {
        // Currently submit draws our geometry directly, not buffered
        // These should be casted to an OpenGL shader with std::dynamic_pointer_cast
        // since they are expected to be opengl gl shaders so we can upload uniform data
        // this way
        shader->setUniformMat4("u_ProjectionView", m_SceneData.camera->getProjectionView());
        shader->setUniformMat4("u_Transform", transform);
        m_RenderCommand.drawIndexed(shader, vertexBuffer, indexBuffer);
    }

    auto Renderer::endScene() -> void {

    }

    auto Renderer::flush() -> void {

    }

    auto Renderer::init() -> void {
        m_RenderCommand.init();
    }
}
