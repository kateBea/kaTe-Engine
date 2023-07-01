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
        VkShaderModuleCreateInfo createInfo{};

        VulkanRenderer& renderer{ *dynamic_cast<VulkanRenderer*>(Renderer::GetCurrentRenderer()) };

        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = srcCode.size();

        // It seems this casts is valid since the default std::vector allocator
        // ensures the data satisfies the worst case alignment requirements.
        createInfo.pCode = reinterpret_cast<const UInt32_T*>(srcCode.data());
        if (vkCreateShaderModule(renderer.m_Device->GetDevice(), &createInfo, nullptr, shaderModule) != VK_SUCCESS)
            throw std::runtime_error("Failed to create shader module");
    }

    auto VulkanShader::Upload(const Path_T& vShaderPath, const Path_T& fShaderPath) -> void {
        // CHECK VULKAN PIPELINE. the shader modules are part of the pipeline, and if we want to recreate the shader modules
        // we may want to recreate the shader modules

        auto vData{ GetFileData(vShaderPath) };
        auto fData{ GetFileData(fShaderPath) };

        KATE_CORE_LOGGER_DEBUG("Loaded vertex shader data. Size {}", vData.size());
        KATE_CORE_LOGGER_DEBUG("Loaded fragment shader data. Size {}", fData.size());

        CreateShaderModule(vData, &m_VertShaderModule);
        CreateShaderModule(fData, &m_FragShaderModule);
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
