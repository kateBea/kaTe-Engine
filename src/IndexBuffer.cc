//
// Created by kate on 6/5/23.
//


#include <Renderer/IndexBuffer.hh>
#include <Renderer/OpenGL/OpenGLIndexBuffer.hh>

#include <Renderer/Renderer.hh>
#include <Tools/Common.hh>
#include <Core/Logger.hh>

namespace kaTe {
    auto IndexBuffer::createBuffer(const std::vector<UInt32_T>& data) -> IndexBuffer* {
        switch(Renderer::getActiveAPI()) {
            case Renderer::GraphicsAPI::OPENGL_API: return new OpenGLIndexBuffer(data);
            default: KATE_CORE_LOGGER_CRITICAL("Unsupported renderer API"); return nullptr;
        }
    }
}


