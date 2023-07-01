//
// Created by kate on 6/30/23.
//

#ifndef KATE_ENGINE_VULKAN_SHADER_HH
#define KATE_ENGINE_VULKAN_SHADER_HH

#include <string_view>

#include <glm/glm.hpp>

#include <volk.h>

#include <Tools/Common.hh>
#include <Renderer/Material/BaseShader.hh>

namespace kaTe {
    class VulkanShader : public BaseShader {
    public:
        /**
         * Default initialization for Shader. DOES NOT Create a valid shader program
         * */
        explicit VulkanShader() = default;

        /**
         * Move constructor
         * */
        VulkanShader(VulkanShader && other) noexcept;

        /**
         * Move assignment
         * @return *this
         * */
        VulkanShader& operator=(VulkanShader&& other) noexcept;

        /**
         * Construct Shader program from path to Vertex Shader source file directory
         * and pixel Shader source file directory
         * @param vertexSourceDir directory to the Vertex Shader source file
         * @param fragmentSourceDir directory to the pixel Shader source file
         * */
        VulkanShader(const Path_T& vertexSourceDir, const std::filesystem::path& fragmentSourceDir);

        /**
         * Loads the shaders specified from paths
         * @param vShaderPath path to vertex shader path
         * @param fShaderPath path to pixel/fragment shader path
         * @throws std::runtime_error exception if any of the shader files could not be opened
         * */
        auto Upload(const Path_T& vShaderPath, const Path_T& fShaderPath) -> void;

        /**
         * Use this Shader program
         * */
        auto Bind() -> void override;

        auto Unbind() -> void override;

    public:
        auto SetBool(std::string_view name, bool value) -> void override;
        auto SetInt(std::string_view name, Int32_T value) -> void override;
        auto SetFloat(std::string_view name, float value) -> void override;
        auto SetVec2(std::string_view name, const glm::vec2& value) -> void override;
        auto SetVec3(std::string_view name, const glm::vec3& value) -> void override;
        auto SetVec4(std::string_view name, const glm::vec4& value) -> void override;
        auto SetMat3(std::string_view name, const glm::mat3& value) -> void override;
        auto SetMat4(std::string_view name, const glm::mat4& value) -> void override;

        ~VulkanShader() override;

    private:
        static auto CreateShaderModule(const CharArray& srcCode, VkShaderModule* shaderModule) -> void;
    private:
        // TODO: fit into and array, there can be more modifiable stages same for OpenGLShader
        VkShaderModule m_VertShaderModule{};
        VkShaderModule m_FragShaderModule{};
    };
}


#endif//KATE_ENGINE_VULKAN_SHADER_HH
