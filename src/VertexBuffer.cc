//
// Created by kate on 6/5/23.
//

#include <Core/Application.hh>
#include <Core/Logger.hh>

#include "Renderer/Buffers/VertexBuffer.hh"
#include <Renderer/OpenGL/OpenGLVertexBuffer.hh>

#include <Renderer/Renderer.hh>

namespace kaTe {
    auto VertexBuffer::CreateBuffer(const std::vector<float>& data) -> std::shared_ptr<VertexBuffer> {
        switch(Renderer::GetActiveGraphicsAPI()) {
            case Renderer::GraphicsAPI::OPENGL_API:
                return std::make_shared<OpenGLVertexBuffer>(data);
            default:
                KATE_CORE_LOGGER_CRITICAL("Unsupported renderer API");
                return nullptr;
        }
    }
}