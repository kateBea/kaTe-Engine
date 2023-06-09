//
// Created by kate on 6/9/23.
//

#include <Renderer/Renderer.hh>
#include <Renderer/RenderCommand.hh>

#include <Renderer/OpenGL/OpenGLRenderer.hh>

namespace kaTe {
    auto RenderCommand::init() -> void {
        switch(Renderer::getActiveAPI()) {
            case Renderer::GraphicsAPI::OPENGL_API:
                s_ActiveRendererAPI = new OpenGLRenderer();
                s_ActiveRendererAPI->init();
                break;
            default:
                KATE_CORE_LOGGER_CRITICAL("Unsupported renderer API");
                break;
        }
    }

    auto RenderCommand::clear(RendererAPI::BufferBit bufferBit) -> void {
        s_ActiveRendererAPI->clear(bufferBit);
    }

    auto RenderCommand::setClearColor(const glm::vec4& color) -> void {
        s_ActiveRendererAPI->setClearColor(color);
    }

    auto RenderCommand::setClearColor(float red, float green, float blue, float alpha) -> void {
        s_ActiveRendererAPI->setClearColor(red, green, blue, alpha);
    }

    auto RenderCommand::drawIndexed(std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer) -> void {
        s_ActiveRendererAPI->drawIndexed(vertexBuffer, indexBuffer);
    }

    auto RenderCommand::drawIndexed(std::shared_ptr<BaseShader> shader, std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer) -> void {
        s_ActiveRendererAPI->drawIndexed(shader, vertexBuffer, indexBuffer);
    }

    auto RenderCommand::draw(std::shared_ptr<VertexBuffer> vertexBuffer) -> void {
        s_ActiveRendererAPI->draw(vertexBuffer);
    }

    auto RenderCommand::draw(std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer) -> void {
        s_ActiveRendererAPI->draw(vertexBuffer, indexBuffer);
    }

    auto RenderCommand::draw(std::shared_ptr<BaseShader> shader, std::shared_ptr<VertexBuffer> vertexBuffer) -> void {
        s_ActiveRendererAPI->draw(shader, vertexBuffer);
    }

    auto RenderCommand::draw(std::shared_ptr<BaseShader> shader, std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer) -> void {
        s_ActiveRendererAPI->draw(shader, vertexBuffer, indexBuffer);
    }

    auto RenderCommand::refreshViewPort(UInt32_T width, UInt32_T height) -> void {
        s_ActiveRendererAPI->setViewPort(width, height);
    }
    auto RenderCommand::shutDown() -> void {
        delete s_ActiveRendererAPI;
    }
}