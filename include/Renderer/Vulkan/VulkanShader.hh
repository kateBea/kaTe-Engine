//
// Created by kate on 7/3/23.
//

#ifndef VULKATE_VULKAN_SHADER_HH
#define VULKATE_VULKAN_SHADER_HH

#include <filesystem>

#include <volk.h>

#include <Tools/Common.hh>
#include <Renderer/Material/BaseShader.hh>
#include <Renderer/Vulkan/VulkanPipeline.hh>

namespace kaTe {
    class VulkanShader : public BaseShader {
    public:
        explicit VulkanShader(ShaderStage stage);

        auto Upload(const Path_T& src) -> void;

        auto Bind() -> void override {}
        auto Unbind() -> void override {}

        auto SetBool(std::string_view name, bool value) -> void override {}
        auto SetInt(std::string_view name, Int32_T value) -> void override {}
        auto SetFloat(std::string_view name, float value) -> void override {}
        auto SetVec2(std::string_view name, const glm::vec2& vec) -> void override {}
        auto SetVec3(std::string_view name, const glm::vec3& vec) -> void override {}
        auto SetVec4(std::string_view name, const glm::vec4& vec) -> void override {}
        auto SetMat3(std::string_view name, const glm::mat3& mat) -> void override {}
        auto SetMat4(std::string_view name, const glm::mat4& mat) -> void override {}

        auto OnRelease() const -> void;

    private:
        static auto GetFileData(const Path_T& path) -> std::vector<char>;

        static auto CreateShaderModule(const std::string &srcCode, VkShaderModule& shaderModule) -> void;
        static auto GetVulkanStageFromShaderStage(ShaderStage stage) -> VkShaderStageFlagBits;

    private:
        struct ShaderInfo {
            ShaderStage                     Stage{};
            std::string                     EntryPoint{ "main" };
            std::string                     SrcPath{};
            VkPipelineShaderStageCreateInfo StageCreateInfo{};

            VkPipelineLayout                PipelineLayout{};
            std::shared_ptr<VulkanPipeline> Pipeline{};

            std::vector<VkDescriptorSetLayout> DescriptorSetLayouts{};
            std::vector<VkDescriptorSetLayout> DescriptorSets{};

            std::vector<VkBuffer> m_UniformBuffers;
            std::vector<VkDeviceMemory> m_UniformBuffersMemory;
            std::vector<void*> m_UniformBuffersMapped;
        };

        ShaderInfo m_Data{};
























    };
}


#endif //VULKATE_VULKAN_SHADER_HH
