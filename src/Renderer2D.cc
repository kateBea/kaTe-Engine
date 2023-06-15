//
// Created by kate on 6/12/23.
//

#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Core/Assert.hh>

#include "Renderer/Renderer2D.hh"
#include <Renderer/RenderCommand.hh>
#include "Renderer/OpenGL/OpenGLShader.hh"

namespace kaTe {

    auto Renderer2D::Init() -> void {
        s_DrawData = std::make_unique<Renderer2DDrawData>();
        KT_ASSERT(s_DrawData, "Renderer 2D draw data pointer is NULL");
        std::vector<float> squareData {
                // Positions            // Texture coordinates
                -0.5f,  -0.5f, 0.0f,     0.0f, 0.0f,   // bottom left
                 0.5f,  -0.5f, 0.0f,     1.0f, 0.0f,   // bottom right
                 0.5f,   0.5f, 0.0f,     1.0f, 1.0f,   // top right
                -0.5f,   0.5f, 0.0f,     0.0f, 1.0f,   // top left
        };

        s_DrawData->vertexBuffer = VertexBuffer::CreateBuffer(squareData);
        s_DrawData->indexBuffer = IndexBuffer::CreateBuffer({0, 1, 2, 2, 3, 0});
        s_DrawData->vertexBuffer->SetBufferLayout(BufferLayout{{ShaderDataType::FLOAT3_TYPE, "a_Position"}, {ShaderDataType::FLOAT2_TYPE, "a_TextureCoordinates"}});
        s_DrawData->colorShader = std::make_shared<OpenGLShader>("../assets/shaders/debugShaderVert.glsl", "../assets/shaders/colorShader.glsl");
        s_DrawData->textureShader = std::make_shared<OpenGLShader>("../assets/shaders/textureVert.glsl", "../assets/shaders/textureFrag.glsl");
        std::dynamic_pointer_cast<OpenGLShader>(s_DrawData->textureShader)->setUniformInt("u_TextSampler", 0);
    }

    auto Renderer2D::BeginScene(std::shared_ptr<OrthographicCamera> camera) -> void {
        s_DrawData->camera = camera;
    }

    auto Renderer2D::EndScene() -> void {

    }

    auto Renderer2D::DrawQuad(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color, double angle) -> void {
        DrawQuad(glm::vec3(position, 0.0), size, color, angle);
    }

    auto Renderer2D::DrawQuad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color, double angle) -> void {

        static constexpr glm::vec3 zAxis{ 0.0f, 0.0f, 1.0f };
        static constexpr glm::mat4 identMat{ glm::mat4(1.0) };

        glm::mat4 scale{ glm::scale(identMat, glm::vec3(size, 1.0f)) };
        glm::mat4 rotation{ glm::rotate(identMat, (float)glm::radians(angle), zAxis) };
        glm::mat4 transform{ glm::translate(identMat, position) * scale * rotation };

        std::dynamic_pointer_cast<OpenGLShader>(s_DrawData->colorShader)->setUniformVec4("u_Color", glm::vec4(color));
        std::dynamic_pointer_cast<OpenGLShader>(s_DrawData->colorShader)->setUniformMat4("u_ProjectionView", s_DrawData->camera->getProjectionView());
        std::dynamic_pointer_cast<OpenGLShader>(s_DrawData->colorShader)->setUniformMat4("u_Transform", transform);

        RenderCommand::DrawIndexed(s_DrawData->colorShader, s_DrawData->vertexBuffer, s_DrawData->indexBuffer);
    }

    auto Renderer2D::ShutDown() -> void {

    }

    auto Renderer2D::DrawQuad(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color, double angle, std::shared_ptr<Texture> texture) -> void {
        DrawQuad(glm::vec3(position, 0.0), size, color, angle, texture);
    }

    auto Renderer2D::DrawQuad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color, double angle, std::shared_ptr<Texture> texture) -> void {
        static constexpr glm::vec3 zAxis{ 0.0f, 0.0f, 1.0f };
        static constexpr glm::mat4 identMat{ glm::mat4(1.0) };

        glm::mat4 scale{ glm::scale(identMat, glm::vec3(size, 1.0f)) };
        glm::mat4 rotation{ glm::rotate(identMat, (float)glm::radians(angle), zAxis) };
        glm::mat4 transform{ glm::translate(identMat, position) * scale * rotation };

        std::dynamic_pointer_cast<OpenGLShader>(s_DrawData->textureShader)->setUniformMat4("u_ProjectionView", s_DrawData->camera->getProjectionView());
        std::dynamic_pointer_cast<OpenGLShader>(s_DrawData->textureShader)->setUniformMat4("u_Transform", transform);
        texture->Bind();
        RenderCommand::DrawIndexed(s_DrawData->textureShader, s_DrawData->vertexBuffer, s_DrawData->indexBuffer);
    }
}