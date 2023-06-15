//
// Created by kate on 6/5/23.
//


#include "Renderer/Buffers/IndexBuffer.hh"
#include <Renderer/OpenGL/OpenGLIndexBuffer.hh>

#include <Core/Application.hh>

#include <Renderer/Renderer.hh>
#include <Tools/Common.hh>
#include <Core/Logger.hh>

namespace kaTe {
    auto IndexBuffer::CreateBuffer(const std::vector<UInt32_T>& data) -> std::shared_ptr<IndexBuffer> {
        switch(Renderer::GetActiveGraphicsAPI()) {
            case Renderer::GraphicsAPI::OPENGL_API:
                return std::make_shared<OpenGLIndexBuffer>(data);
            default:
                KATE_CORE_LOGGER_CRITICAL("Unsupported renderer API");
                return nullptr;
        }
    }
}


