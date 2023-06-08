//
// Created by kate on 6/8/23.
//

#include <memory>

#include <Core/EngineManager.hh>

#include <Renderer/Renderer.hh>

#include "Renderer/Material/Texture2D.hh"
#include <Renderer/OpenGL/OpenGLTexture2D.hh>
#include <Core/Logger.hh>

namespace kaTe {

    auto Texture2D::createTexture(const Path_T &path) -> Texture* {
        EngineManager& engine{ EngineManager::get() };
        switch(engine.getRenderer()->getActiveAPI()) {
            case Renderer::GraphicsAPI::OPENGL_API: return new OpenGLTexture2D(path);
            default: KATE_CORE_LOGGER_CRITICAL("Unsupported renderer API"); return nullptr;
        }
    }
}