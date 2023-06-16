//
// Created by kate on 6/16/23.
//

#include <Core/Logger.hh>

#include <Renderer/OpenGL/OpenGLShader.hh>
#include <Renderer/Material/BaseShader.hh>
#include <Renderer/Renderer.hh>

namespace kaTe {

    auto BaseShader::CreateShader(const std::filesystem::path &vertStage, const std::filesystem::path &pixelStage) -> std::shared_ptr<BaseShader> {
        switch(Renderer::GetActiveGraphicsAPI()) {
            case Renderer::GraphicsAPI::OPENGL_API:
                return std::make_shared<OpenGLShader>(vertStage, pixelStage);
            default:
                KATE_CORE_LOGGER_CRITICAL("Unsupported renderer API");
                return nullptr;
        }
    }
}