/**
 * StandardMaterial.hh
 * Created by kate on 6/30/23.
 * */

#ifndef KATE_ENGINE_STANDARD_MATERIAL_HH
#define KATE_ENGINE_STANDARD_MATERIAL_HH

// C++ Standard Library
#include <string>
#include <string_view>
#include <memory>

// Third-Party Libraries
#include <glm/glm.hpp>

// Project Headers
#include <Tools/Common.hh>
#include <Renderer/Material/BaseShader.hh>
#include <Renderer/Material/Material.hh>
#include <Renderer/Material/Texture.hh>

namespace kaTe {
    class StandardMaterial : public Material {
    public:
        explicit StandardMaterial(std::string_view name = "StandardMaterial");

        StandardMaterial(const StandardMaterial& other) = default;
        StandardMaterial(StandardMaterial&& other) = default;

        auto operator=(const StandardMaterial& other) -> StandardMaterial& = default;
        auto operator=(StandardMaterial&& other) -> StandardMaterial& = default;

        KT_NODISCARD auto GetColor() const -> const glm::vec4& { return m_Color; }
        KT_NODISCARD auto GetTextSampler() const -> UInt32_T { return m_TextureSampler; }

        auto SetColor(const glm::vec4& color) -> void { m_Color = color; }
        auto SetTextSampler(UInt32_T sampler) -> void { m_TextureSampler = sampler; }

        auto Use() -> void override;

    public:
        // Shader uniforms
        glm::vec4 m_Color{};
        UInt32_T m_TextureSampler{};

        std::shared_ptr<BaseShader> m_Shader{};
        std::shared_ptr<Texture> m_Texture{};
    };
}


#endif//KATE_ENGINE_STANDARD_MATERIAL_HH
