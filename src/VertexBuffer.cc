//
// Created by kate on 6/5/23.
//

#include <Renderer/OpenGL/OpenGLVertexBuffer.hh>
#include <Renderer/VertexBuffer.hh>

#include <Renderer/Renderer.hh>
#include <Core/Logger.hh>

namespace kaTe {
    auto VertexBuffer::createBuffer(const std::vector<float>& data) -> VertexBuffer* {
        switch(Renderer::getActiveAPI()) {
            case Renderer::GraphicsAPI::OPENGL_API: return new OpenGLVertexBuffer(data);
            default: KATE_CORE_LOGGER_CRITICAL("Unsupported renderer API"); return nullptr;
        }
    }
}