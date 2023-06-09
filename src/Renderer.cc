//
// Created by kate on 6/5/23.
//

#include <Renderer/Renderer.hh>
#include <Renderer/RenderCommand.hh>
#include <Renderer/OpenGL/OpenGLShader.hh>

namespace kaTe {
    auto Renderer::beginScene(std::shared_ptr<OrthographicCamera> camera) -> void {
        s_SceneData.camera = camera;
    }

    auto Renderer::submit(std::shared_ptr<VertexBuffer> vertexBuffer) -> void {

    }

    auto Renderer::submit(std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer) -> void {
        RenderCommand::drawIndexed(vertexBuffer, indexBuffer);
    }

    auto Renderer::submit(std::shared_ptr<BaseShader> shader, std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer, const glm::mat4 &transform) -> void {
        // Currently submit draws our geometry directly, not buffered
        // These should be casted to an OpenGL shader with std::dynamic_pointer_cast
        // since they are expected to be opengl gl shaders, so we can upload uniform data
        // this way
        std::dynamic_pointer_cast<OpenGLShader>(shader)->setUniformMat4("u_ProjectionView", s_SceneData.camera->getProjectionView());
        std::dynamic_pointer_cast<OpenGLShader>(shader)->setUniformMat4("u_Transform", transform);
        RenderCommand::drawIndexed(shader, vertexBuffer, indexBuffer);
    }

    auto Renderer::endScene() -> void {

    }

    auto Renderer::flush() -> void {

    }

    auto Renderer::init() -> void {

    }

    auto Renderer::shutDown() -> void {

    }
}
