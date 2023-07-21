//
// Created by kate on 7/21/2023.
//

#ifndef KATE_ENGINE_RENDERING_UTILITIES_HH
#define KATE_ENGINE_RENDERING_UTILITIES_HH

#include <glm/glm.hpp>

#include <Renderer/Material/Material.hh>
#include <Renderer/Material/Texture.hh>
#include <Renderer/Buffers/IndexBuffer.hh>
#include <Renderer/Buffers/VertexBuffer.hh>

namespace kaTe {
    struct UniformTransformData {
        glm::mat4 ProjectionView{};
        glm::mat4 Transform{};
        glm::mat4 Projection{};
        glm::mat4 View{};
    };

    struct RenderingData {
        std::shared_ptr<VertexBuffer> VertexBufferData{};
        std::shared_ptr<IndexBuffer> IndexBufferData{};
        std::shared_ptr<Material> MaterialData{};
        std::shared_ptr<Texture> TextureData{};
        UniformTransformData TransformData{};
        glm::vec4 Color{};
    };
}

#endif//KATE_ENGINE_RENDERING_UTILITIES_HH
