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

    auto OpenGLRenderer::Clear(const BufferBits& bufferBits) -> void {
        GLbitfield mask{};
        using BufferBit = RendererAPI::BufferBit;

        if (bufferBits[BufferBit::DEPTH_BUFFER_BIT]) mask = GL_DEPTH_BUFFER_BIT;
        if (bufferBits[BufferBit::COLOR_BUFFER_BIT]) mask |= GL_COLOR_BUFFER_BIT;

        glClear(mask);
    }

    auto OpenGLRenderer::SetClearColor(const glm::vec4 &color) -> void {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    auto OpenGLRenderer::SetViewPort(UInt32_T x, UInt32_T y, UInt32_T width, UInt32_T height) -> void {
        glViewport(x, y, width, height);
    }

    auto OpenGLRenderer::DrawIndexed([[maybe_unused]] const std::shared_ptr<VertexBuffer> &vertexBuffer, [[maybe_unused]] const std::shared_ptr<IndexBuffer> &indexBuffer) -> void {

    }

    auto OpenGLRenderer::DrawIndexed(const std::shared_ptr<BaseShader> &shader, const std::shared_ptr<VertexBuffer> &vertexBuffer, const std::shared_ptr<IndexBuffer> &indexBuffer) -> void {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
        std::dynamic_pointer_cast<OpenGLShader>(shader)->Bind();
        m_VertexArray.UseVertexBuffer(vertexBuffer);
        indexBuffer->BindBuffer();

        glDrawElements(GL_TRIANGLES, indexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);
    }

    auto OpenGLRenderer::Draw(const std::shared_ptr<VertexBuffer> &vertexBuffer) -> void {

    }

    auto OpenGLRenderer::Draw(const std::shared_ptr<VertexBuffer>& vertexBuffer, const std::shared_ptr<IndexBuffer>& indexBuffer) -> void {

    }

    auto OpenGLRenderer::Draw(const std::shared_ptr<BaseShader>& shader, const std::shared_ptr<VertexBuffer>& vertexBuffer) -> void {

    }

    auto OpenGLRenderer::Draw(const std::shared_ptr<BaseShader>& shader, const std::shared_ptr<VertexBuffer>& vertexBuffer, const std::shared_ptr<IndexBuffer> &indexBuffer) -> void {

    }

    auto OpenGLRenderer::Init() -> void {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        KATE_CORE_LOGGER_INFO("Render blending enabled");
    }

    auto OpenGLRenderer::Shutdown() -> void {

    }

    auto OpenGLRenderer::SetDefaultShader(const Path_T &vertShaderPath, const Path_T &pixelShaderPath) -> void {
        m_DefaultVertexPixelShaders.Upload(vertShaderPath, pixelShaderPath);
    }

    auto OpenGLRenderer::EnableWireframeMode() -> void {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    auto OpenGLRenderer::DisableWireframeMode() -> void {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}