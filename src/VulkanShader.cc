//
// Created by kate on 6/30/23.
//
// C++ Standard Library
#include <string_view>

// Third-Party Libraries
#include <volk.h>
#include <glm/glm.hpp>

// Project Headers
#include "Renderer/Vulkan/VulkanRenderer.hh"
#include <Core/Assert.hh>
#include <Core/Logger.hh>
#include <Renderer/Renderer.hh>
#include <Renderer/Vulkan/VulkanShader.hh>
#include <Tools/Common.hh>

namespace kaTe {

    VulkanShader::VulkanShader(VulkanShader &&other) noexcept {

    }

    VulkanShader &VulkanShader::operator=(VulkanShader &&other) noexcept {

        return *this;
    }

    VulkanShader::VulkanShader(const Path_T& vertexSourceDir, const Path_T& fragmentSourceDir) {
        Upload(vertexSourceDir, fragmentSourceDir);
    }

    auto VulkanShader::CreateShaderModule(const CharArray& srcCode, VkShaderModule* shaderModule) -> void {
        VulkanRenderer& renderer{ *dynamic_cast<VulkanRenderer*>(Renderer::GetCurrentRenderer()) };

        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = srcCode.size();

        // It seems this casts is valid since the default std::vector allocator
        // ensures the data satisfies the worst case alignment requirements.
        createInfo.pCode = reinterpret_cast<const UInt32_T*>(srcCode.data());
        if (vkCreateShaderModule(renderer.m_Device, &createInfo, nullptr, shaderModule) != VK_SUCCESS)
            throw std::runtime_error("Failed to create shader module");
    }

    auto VulkanShader::Upload(const Path_T& vShaderPath, const Path_T& fShaderPath) -> void {
        auto vData{ GetFileData(vShaderPath) };
        auto fData{ GetFileData(fShaderPath) };

        KATE_CORE_LOGGER_DEBUG("Loaded vertex shader data. Size {}", vData.size());
        KATE_CORE_LOGGER_DEBUG("Loaded fragment shader data. Size {}", fData.size());

        CreateShaderModule(vData, &m_VertShaderModule);
        CreateShaderModule(fData, &m_FragShaderModule);

        std::array<VkPipelineShaderStageCreateInfo, 2> shaderStages{};

        shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
        shaderStages[0].module = m_VertShaderModule;
        shaderStages[0].pName = "main";
        shaderStages[0].flags = 0;
        shaderStages[0].pNext = nullptr;
        shaderStages[0].pSpecializationInfo = nullptr;

        shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        shaderStages[1].module = m_FragShaderModule;
        shaderStages[1].pName = "main";
        shaderStages[1].flags = 0;
        shaderStages[1].pNext = nullptr;
        shaderStages[1].pSpecializationInfo = nullptr;
    }

    auto VulkanShader::Bind() -> void {

    }

    auto VulkanShader::Unbind() -> void {

    }

    auto VulkanShader::SetBool(std::string_view name, bool value) -> void {

    }

    auto VulkanShader::SetInt(std::string_view name, Int32_T value) -> void {

    }

    auto VulkanShader::SetFloat(std::string_view name, float value) -> void {

    }

    auto VulkanShader::SetVec2(std::string_view name, const glm::vec2 &value) -> void {

    }

    auto VulkanShader::SetVec3(std::string_view name, const glm::vec3 &value) -> void {

    }

    auto VulkanShader::SetVec4(std::string_view name, const glm::vec4 &value) -> void {

    }

    auto VulkanShader::SetMat3(std::string_view name, const glm::mat3 &value) -> void {

    }

    auto VulkanShader::SetMat4(std::string_view name, const glm::mat4 &value) -> void {

    }

    VulkanShader::~VulkanShader() {

    }

}
