//
// Created by kate on 6/5/23.
//

#include <Renderer/Renderer.hh>
#include <Renderer/RenderCommand.hh>

namespace kaTe {
    auto Renderer::beginScene() -> void {

    }

    auto Renderer::submit(std::shared_ptr<VertexBuffer> vertexBuffer) -> void {

    }

    auto Renderer::submit(std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer) -> void {
        RenderCommand::drawIndexed(vertexBuffer, indexBuffer);
    }

    auto Renderer::submit(std::shared_ptr<Shader> shader, std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer) -> void {
        RenderCommand::drawIndexed(shader, vertexBuffer, indexBuffer);
    }

    auto Renderer::endScene() -> void {

    }

    auto Renderer::flush() -> void {

    }
}
