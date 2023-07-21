//
// Created by kate on 7/20/2023.
//

#include <Renderer/OpenGL/OpenGLDefaultMaterial.hh>

namespace kaTe {

    auto OpenGLDefaultMaterial::BindShader() -> void {

    }

    auto OpenGLDefaultMaterial::UnbindShader() -> void {

    }

    auto OpenGLDefaultMaterial::BindTexture() -> void {
        m_Texture->Bind(0);
    }

    auto OpenGLDefaultMaterial::UnbindTexture() -> void {

    }

    auto OpenGLDefaultMaterial::UploadShaders(const Path_T &vertexShader, const Path_T &fragmentShader) -> void {
        m_DefaultVertexPixelShaders = std::dynamic_pointer_cast<OpenGLShader>(
                BaseShader::CreateShader(vertexShader,
                                         fragmentShader));
    }

    auto OpenGLDefaultMaterial::SetTextureSampler(Int32_T value) -> void {
        m_DefaultVertexPixelShaders->SetInt("u_TextSampler", value);
    }

    auto OpenGLDefaultMaterial::SetTiltingColor(float red, float green, float blue, float alpha) -> void {

    }

    auto OpenGLDefaultMaterial::SetProjectionView(const glm::mat4 &mat) -> void {

    }

    auto OpenGLDefaultMaterial::SetTransform(const glm::mat4 &mat) -> void {

    }
}
