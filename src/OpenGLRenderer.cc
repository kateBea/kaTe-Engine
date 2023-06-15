//
// Created by kate on 6/6/23.
//

#include <memory>
#include <GL/glew.h>

#include <Core/Logger.hh>
#include <Renderer/OpenGL/OpenGLShader.hh>
#include <Renderer/OpenGL/OpenGLRenderer.hh>

namespace kaTe {
    auto OpenGLRenderer::SetClearColor(float red, float green, float blue, float alpha) -> void {
        glClearColor(red, green, blue, alpha);
    }

    auto OpenGLRenderer::Clear(BufferBit bufferBits) -> void {
        glClear(bufferBits);
    }

    auto OpenGLRenderer::SetClearColor(const glm::vec4 &color) -> void {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    auto OpenGLRenderer::SetViewPort(UInt32_T x, UInt32_T y, UInt32_T width, UInt32_T height) -> void {
        glViewport(x, y, width, height);
    }

    auto OpenGLRenderer::DrawIndexed(std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer) -> void {


    }

    auto OpenGLRenderer::DrawIndexed(std::shared_ptr<BaseShader> shader, std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer) -> void {
        std::dynamic_pointer_cast<OpenGLShader>(shader)->bind();
        m_VertexArray.useVertexBuffer(vertexBuffer);
        indexBuffer->BindBuffer();

        glDrawElements(GL_TRIANGLES, indexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);
    }

    auto OpenGLRenderer::Draw(std::shared_ptr<VertexBuffer> vertexBuffer) -> void {

    }

    auto OpenGLRenderer::Draw(std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer) -> void {

    }

    auto OpenGLRenderer::Draw(std::shared_ptr<BaseShader> shader, std::shared_ptr<VertexBuffer> vertexBuffer) -> void {

    }

    auto OpenGLRenderer::Draw(std::shared_ptr<BaseShader> shader, std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer) -> void {

    }

    auto OpenGLRenderer::Init() -> void {
        glEnable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        KATE_CORE_LOGGER_INFO("Render blending enabled");
    }

    auto OpenGLRenderer::Shutdown() -> void {

    }
}