//
// Created by kate on 7/20/2023.
//

#include <Renderer/OpenGL/OpenGLDefaultMaterial.hh>

namespace kaTe {

    auto OpenGLDefaultMaterial::BindShader() -> void {
        m_DefaultVertexPixelShaders->Bind();
    }

    auto OpenGLDefaultMaterial::UnbindShader() -> void {
        OpenGLShader::Unbind();
    }

    auto OpenGLDefaultMaterial::BindTexture() -> void {
        m_Texture->Bind(0);
    }

    auto OpenGLDefaultMaterial::UnbindTexture() -> void {

    }

    auto OpenGLDefaultMaterial::UploadShaders(const Path_T &vertexShader, const Path_T &fragmentShader) -> void {
        m_DefaultVertexPixelShaders = std::dynamic_pointer_cast<OpenGLShader>(
                Shader::CreateShader(vertexShader,
                                         fragmentShader));
    }

    auto OpenGLDefaultMaterial::SetTextureSampler(Int32_T value) -> void {
        m_DefaultVertexPixelShaders->SetUniformInt("u_TextSampler", value);
    }

    auto OpenGLDefaultMaterial::SetTiltingColor(float red, float green, float blue, float alpha) -> void {
        m_DefaultVertexPixelShaders->SetUniformVec4("u_Color", {red, green, blue, alpha});
    }

    auto OpenGLDefaultMaterial::SetTiltingColor(const glm::vec4& color) -> void {
        m_DefaultVertexPixelShaders->SetUniformVec4("u_Color", color);
    }

    auto OpenGLDefaultMaterial::SetProjectionView(const glm::mat4 &mat) -> void {
        m_DefaultVertexPixelShaders->SetUniformMat4("u_ProjectionView", mat);
    }

    auto OpenGLDefaultMaterial::SetTransform(const glm::mat4 &mat) -> void {
        m_DefaultVertexPixelShaders->SetUniformMat4("u_Transform", mat);
    }
}
