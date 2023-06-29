//
// Created by kate on 6/9/23.
//

#include <Core/Assert.hh>

#include <Renderer/Renderer.hh>
#include <Renderer/RenderCommand.hh>

#include <Renderer/OpenGL/OpenGLRenderer.hh>

namespace kaTe {
    auto RenderCommand::Init() -> void {
        switch(Renderer::GetActiveGraphicsAPI()) {
            case Renderer::GraphicsAPI::OPENGL_API:
                s_ActiveRendererAPI = new OpenGLRenderer();
                s_ActiveRendererAPI->Init();
                break;
            default:
                KATE_CORE_LOGGER_CRITICAL("Unsupported renderer API");
                break;
        }
    }

    auto RenderCommand::Clear(BufferBits bufferBits) -> void {
        KT_ASSERT(s_ActiveRendererAPI, "Render command active API is NULL");
        s_ActiveRendererAPI->Clear(bufferBits);
    }

    auto RenderCommand::SetClearColor(const glm::vec4& color) -> void {
        KT_ASSERT(s_ActiveRendererAPI, "Render command active API is NULL");
        s_ActiveRendererAPI->SetClearColor(color);
    }

    auto RenderCommand::SetClearColor(float red, float green, float blue, float alpha) -> void {
        KT_ASSERT(s_ActiveRendererAPI, "Render command active API is NULL");
        s_ActiveRendererAPI->SetClearColor(red, green, blue, alpha);
    }

    auto RenderCommand::DrawIndexed(const std::shared_ptr<VertexBuffer>& vertexBuffer, const std::shared_ptr<IndexBuffer>& indexBuffer) -> void {
        KT_ASSERT(s_ActiveRendererAPI, "Render command active API is NULL");
        s_ActiveRendererAPI->DrawIndexed(vertexBuffer, indexBuffer);
    }

    auto RenderCommand::DrawIndexed(const std::shared_ptr<BaseShader>& shader, const std::shared_ptr<VertexBuffer>& vertexBuffer, const std::shared_ptr<IndexBuffer>& indexBuffer) -> void {
        KT_ASSERT(s_ActiveRendererAPI, "Render command active API is NULL");
        s_ActiveRendererAPI->DrawIndexed(shader, vertexBuffer, indexBuffer);
    }

    auto RenderCommand::Draw(const std::shared_ptr<VertexBuffer>& vertexBuffer) -> void {
        KT_ASSERT(s_ActiveRendererAPI, "Render command active API is NULL");
        s_ActiveRendererAPI->Draw(vertexBuffer);
    }

    auto RenderCommand::Draw(const std::shared_ptr<VertexBuffer>& vertexBuffer, const std::shared_ptr<IndexBuffer>& indexBuffer) -> void {
        KT_ASSERT(s_ActiveRendererAPI, "Render command active API is NULL");
        s_ActiveRendererAPI->Draw(vertexBuffer, indexBuffer);
    }

    auto RenderCommand::Draw(const std::shared_ptr<BaseShader>& shader, const std::shared_ptr<VertexBuffer>& vertexBuffer) -> void {
        KT_ASSERT(s_ActiveRendererAPI, "Render command active API is NULL");
        s_ActiveRendererAPI->Draw(shader, vertexBuffer);
    }

    auto RenderCommand::Draw(const std::shared_ptr<BaseShader>& shader, const std::shared_ptr<VertexBuffer>& vertexBuffer, const std::shared_ptr<IndexBuffer>& indexBuffer) -> void {
        KT_ASSERT(s_ActiveRendererAPI, "Render command active API is NULL");
        s_ActiveRendererAPI->Draw(shader, vertexBuffer, indexBuffer);
    }

    auto RenderCommand::UpdateViewPort(UInt32_T x, UInt32_T y, UInt32_T width, UInt32_T height) -> void {
        KT_ASSERT(s_ActiveRendererAPI, "Render command active API is NULL");
        s_ActiveRendererAPI->SetViewPort(x, y, width, height);
    }
    auto RenderCommand::ShutDown() -> void {
        delete s_ActiveRendererAPI;
    }

    auto RenderCommand::EnableWireframeMode() -> void {
        s_ActiveRendererAPI->EnableWireframeMode();
    }

    auto RenderCommand::DisableWireframeMode() -> void {
        s_ActiveRendererAPI->DisableWireframeMode();
    }
    auto RenderCommand::GetSwapChain() -> std::any {
        return s_ActiveRendererAPI->GetSwapChain();
    }
    auto RenderCommand::GetCommandBuffers() -> std::any {
        return s_ActiveRendererAPI->GetCommandBuffers();
    }
}