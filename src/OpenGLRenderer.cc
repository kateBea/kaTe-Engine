//
// Created by kate on 6/6/23.
//

#include <memory>

#include <GL/glew.h>

#include <Renderer/OpenGL/OpenGLRenderer.hh>

namespace kaTe {
    auto OpenGLRenderer::SetClearColor(float red, float green, float blue, float alpha) -> void {
        glClearColor(red, green, blue, alpha);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    }

    auto OpenGLRenderer::SetClearColor(const glm::vec4 &color) -> void {
        glClearColor(color.r, color.g, color.b, color.a);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    }

    auto OpenGLRenderer::SetViewPort(UInt32_T x, UInt32_T y, UInt32_T width, UInt32_T height) -> void {
        glViewport((GLsizei)x, (GLsizei)y, (GLsizei)width, (GLsizei)height);
    }

    auto OpenGLRenderer::DrawIndexed(const std::shared_ptr<VertexBuffer> &vertexBuffer, const std::shared_ptr<IndexBuffer> &indexBuffer) -> void {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
        m_DefaultMaterial.BindShader();
        m_VertexArray.UseVertexBuffer(vertexBuffer);
        indexBuffer->Bind();

        glDrawElements(GL_TRIANGLES, (GLsizei)indexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);
    }

    auto OpenGLRenderer::Draw(const RenderingData& data) -> void {

        m_DefaultMaterial.GetShader()->SetVec4("u_Color", data.Color);
        m_DefaultMaterial.GetShader()->SetMat4("u_ProjectionView", data.TransformData.ProjectionView);
        m_DefaultMaterial.GetShader()->SetMat4("u_Transform", data.TransformData.Transform);

        if (data.TextureData != nullptr) {
            m_DefaultMaterial.SetTextureSampler(0);
            std::dynamic_pointer_cast<OpenGLTexture2D>(data.TextureData)->Bind(0);
        }

        if (data.IndexBufferData != nullptr)
            DrawIndexed(data.VertexBufferData, data.IndexBufferData);
    }

    auto OpenGLRenderer::Init() -> void {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        m_DefaultMaterial.UploadShaders("../assets/shaders/debugShaderVert.glsl", "../assets/shaders/debugShaderFrag.glsl");
    }

    auto OpenGLRenderer::Shutdown() -> void {

    }

    auto OpenGLRenderer::EnableWireframeMode() -> void {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    auto OpenGLRenderer::DisableWireframeMode() -> void {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    auto OpenGLRenderer::OnEvent(Event& event) -> void {

    }

}