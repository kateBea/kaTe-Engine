//
// Created by kate on 6/6/23.
//

#include <memory>
#include <GL/glew.h>

#include <Core/Logger.hh>
#include <Renderer/OpenGL/OpenGLShader.hh>
#include <Renderer/OpenGL/OpenGLRenderer.hh>

namespace kaTe {
    auto OpenGLRenderer::setClearColor(float red, float green, float blue, float alpha) -> void {
        glClearColor(red, green, blue, alpha);
    }

    auto OpenGLRenderer::clear(BufferBit bufferBits) -> void {
        glClear(bufferBits);
    }

    auto OpenGLRenderer::setClearColor(const glm::vec4 &color) -> void {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    auto OpenGLRenderer::setViewPort(UInt32_T width, UInt32_T height) -> void {
        glViewport(0, 0, width, height);
    }

    auto OpenGLRenderer::drawIndexed(std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer) -> void {


    }

    auto OpenGLRenderer::drawIndexed(std::shared_ptr<BaseShader> shader, std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer) -> void {
        std::dynamic_pointer_cast<OpenGLShader>(shader)->bind();
        m_VertexArray.useVertexBuffer(vertexBuffer);
        indexBuffer->bindIndexBuffer();

        glDrawElements(GL_TRIANGLES, indexBuffer->getCount(), GL_UNSIGNED_INT, nullptr);
    }

    auto OpenGLRenderer::draw(std::shared_ptr<VertexBuffer> vertexBuffer) -> void {

    }

    auto OpenGLRenderer::draw(std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer) -> void {

    }

    auto OpenGLRenderer::draw(std::shared_ptr<BaseShader> shader, std::shared_ptr<VertexBuffer> vertexBuffer) -> void {

    }

    auto OpenGLRenderer::draw(std::shared_ptr<BaseShader> shader, std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer) -> void {

    }

    auto OpenGLRenderer::init() -> void {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        KATE_CORE_LOGGER_INFO("Render blending enabled");
    }
}