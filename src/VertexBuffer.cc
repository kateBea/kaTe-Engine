//
// Created by kate on 6/5/23.
//

#include "Renderer/Buffers/VertexBuffer.hh"
#include <Renderer/OpenGL/OpenGLVertexBuffer.hh>

#include <Core/EngineManager.hh>

#include <Renderer/Renderer.hh>
#include <Core/Logger.hh>

namespace kaTe {
    auto VertexBuffer::createBuffer(const std::vector<float>& data) -> VertexBuffer* {
        EngineManager& engine{ EngineManager::get() };
        switch(Renderer::getActiveAPI()) {
            case Renderer::GraphicsAPI::OPENGL_API:
                return new OpenGLVertexBuffer(data);
            default:
                KATE_CORE_LOGGER_CRITICAL("Unsupported renderer API");
                return nullptr;
        }
    }
}