//
// Created by kate on 6/8/23.
//

#ifndef KATE_ENGINE_BASE_SHADER_HH
#define KATE_ENGINE_BASE_SHADER_HH

#include <filesystem>

#include <glm/glm.hpp>

namespace kaTe {
    class BaseShader {
    public:
        BaseShader() = default;
        virtual ~BaseShader() = default;

        virtual auto Bind() -> void = 0;
        virtual auto Unbind() -> void = 0;

        virtual auto SetBool(std::string_view name, bool value) -> void = 0;
        virtual auto SetInt(std::string_view name, Int32_T value) -> void = 0;
        virtual auto SetFloat(std::string_view name, float value) -> void = 0;
        virtual auto SetVec2(std::string_view name, const glm::vec2& vec) -> void = 0;
        virtual auto SetVec3(std::string_view name, const glm::vec3& vec) -> void = 0;
        virtual auto SetVec4(std::string_view name, const glm::vec4& vec) -> void = 0;
        virtual auto SetMat3(std::string_view name, const glm::mat3& mat) -> void = 0;
        virtual auto SetMat4(std::string_view name, const glm::mat4& mat) -> void = 0;

        static auto CreateShader(const std::filesystem::path& vertStage, const std::filesystem::path& pixelStage) -> std::shared_ptr<BaseShader>;
    };
}


#endif//KATE_ENGINE_BASE_SHADER_HH
