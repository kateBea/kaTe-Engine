
// C++ Standard Libraries
#include <filesystem>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <string>

// Third-Party Libraries
#include <GL/glew.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fmt/core.h>

// Project Libraries
#include <Core/Logger.hh>
#include <Tools/Common.hh>
#include <Renderer/OpenGL/OpenGLShader.hh>


namespace kaTe {
    OpenGLShader::OpenGLShader(const std::filesystem::path& vertexSourceDir, const std::filesystem::path& fragmentSourceDir) {
        m_Id = glCreateProgram();
        if (m_Id == 0)
            throw std::runtime_error("Error when creating shader program");

        m_ValidId = true;
        load(vertexSourceDir, fragmentSourceDir);
    }

    auto OpenGLShader::load(const std::filesystem::path& vShaderPath, const std::filesystem::path& fShaderPath) -> void {
        if (!m_ValidId) {
            // Check, in case this shader does not contain a valid
            // OpenGL Shader identifier
            m_Id = glCreateProgram();
            if (m_Id == 0)
                throw std::runtime_error("Error when creating shader program");
        }

        auto vertexShaderData{ GetFileData(vShaderPath) };
        auto fragmentShaderData{ GetFileData(fShaderPath) };

        build(vertexShaderData.c_str() , fragmentShaderData.c_str());
    }

    auto OpenGLShader::GetFileData(const std::filesystem::path &path) -> std::string {
        std::ifstream file{ path, std::ios::binary };

        if (!file.is_open())
            throw std::runtime_error("Failed to open Shader file for OpenGL");

        return std::string { std::istreambuf_iterator<std::string::value_type>(file), std::istreambuf_iterator<std::string::value_type>() };
    }

    auto OpenGLShader::compile(const char* content, GLenum shaderType) -> UInt32_T {
        UInt32_T shaderId{};
        shaderId = glCreateShader(shaderType);
        glShaderSource(shaderId, 1, &content, nullptr);
        glCompileShader(shaderId);

        return shaderId;
    }

    auto OpenGLShader::build(const char* vShader, const char* fShader) const -> void {
        UInt32_T vertexShaderID{ compile(vShader, GL_VERTEX_SHADER) };
        showShaderStatus(vertexShaderID, ShaderType::VERTEX_SHADER_TYPE, GL_COMPILE_STATUS);

        UInt32_T pixelShaderID{ compile(fShader, GL_FRAGMENT_SHADER) };
        showShaderStatus(pixelShaderID, ShaderType::FRAGMENT_SHADER_TYPE, GL_COMPILE_STATUS);

        // Create and link program against compiled Shader binaries
        glAttachShader(getProgram(), vertexShaderID);
        glAttachShader(getProgram(), pixelShaderID);
        glLinkProgram(getProgram());

        showProgramStatus(GL_LINK_STATUS);

        // cleanup
        KT_COLOR_PRINT_FORMATTED(KT_FMT_COLOR_LIME_GREEN, "Performing binaries cleanup...\n");
        glDetachShader(getProgram(), vertexShaderID);
        glDetachShader(getProgram(), pixelShaderID);
        glDeleteShader(vertexShaderID);
        glDeleteShader(pixelShaderID);
    }

    auto OpenGLShader::SetUniformBool(std::string_view name, bool value) -> void {
        Bind();
        auto ret{ glGetUniformLocation(getProgram(), name.data()) };
        if (ret == -1)
            KATE_CORE_LOGGER_ERROR("Error: [{}] is not a valid uniform name for this program shader", name);
        else
            glUniform1i(ret, static_cast<Int32_T>(value));
    }

    auto OpenGLShader::SetUniformInt(std::string_view name, Int32_T value) -> void {
        Bind();
        auto ret{ glGetUniformLocation(getProgram(), name.data()) };
        if (ret == -1)
            KATE_CORE_LOGGER_ERROR("Error: [{}] is not a valid uniform name for this program shader", name);
        else
            glUniform1i(ret, value);
    }

    auto OpenGLShader::SetUniformFloat(std::string_view name, float value) -> void {
        Bind();
        auto ret{ glGetUniformLocation(getProgram(), name.data()) };
        if (ret == -1)
            KATE_CORE_LOGGER_ERROR("Error: [{}] is not a valid uniform name for this program shader", name);
        else
            glUniform1f(ret, value);
    }

    auto OpenGLShader::showShaderStatus(UInt32_T objectId, ShaderType type, GLenum status) const -> void {
        Int32_T success{};

        glGetShaderiv(objectId, status, &success);
        switch (status) {
            case GL_COMPILE_STATUS:
                if (success == GL_FALSE) {
                    Int32_T length{};
                    glGetShaderiv(objectId, GL_INFO_LOG_LENGTH, &length);
                    std::string outStr(length, '\0');

                    glGetShaderInfoLog(objectId, length, &length, outStr.data());
                    KATE_CORE_LOGGER_ERROR("Error on {} shader compilation", getShaderTypeStr(type));
                    KT_COLOR_PRINT_FORMATTED(KT_FMT_COLOR_RED, "\n{}", outStr);
                }
                else
                    KT_COLOR_PRINT_FORMATTED(KT_FMT_COLOR_LIME_GREEN, "Shader compilation successful. Type: {}\n", getShaderTypeStr(type));
                break;
        }
    }

    auto OpenGLShader::showProgramStatus(GLenum status) const -> void {
        std::string outStr(1024, '\0');
        Int32_T success{};

        glGetProgramiv(m_Id, status, &success);
        switch (status) {
            case GL_LINK_STATUS:
                if (success == GL_FALSE) {
                    glGetProgramInfoLog(m_Id, outStr.size(), nullptr, outStr.data());
                    KATE_CORE_LOGGER_ERROR("Error on shader program Linking");
                    KT_COLOR_PRINT_FORMATTED(KT_FMT_COLOR_RED, "\n{}", outStr);
                }
                else
                    KATE_CORE_LOGGER_INFO("Shader program linking successful");

                break;
        }
    }

    auto OpenGLShader::SetUniformMat4(std::string_view name, const glm::mat4& value) -> void {
        Bind();
        auto ret{ glGetUniformLocation(getProgram(), name.data()) };
        if (ret == -1)
            KATE_CORE_LOGGER_ERROR("Error: [{}] is not a valid uniform name for this program shader", name);
        else
            /*
             * If transpose is GL_FALSE, each matrix is assumed to be supplied in column major order.
             * If transpose is GL_TRUE, each matrix is assumed to be supplied in row major order.
             * The count argument indicates the number of matrices to be passed. A count of 1
             * should be used if modifying the value of a single matrix, and a count greater
             * than 1 can be used to modify an array of matrices.
             * from: https://docs.gl/gl4/glUniform
             *
             * we pass GL_FALSE because glm::mat4 has each row stored contiguously in memory by default,
             * meaning the elements of the first row are stored first, followed by the
             * elements of the second row, and so on.
             * */
            glUniformMatrix4fv(ret, 1, GL_FALSE, glm::value_ptr(value));
    }

    auto OpenGLShader::SetUniformVec3(std::string_view name, const glm::vec3 &value) -> void {
        Bind();
        auto ret{ glGetUniformLocation(getProgram(), name.data()) };
        if (ret == -1)
            KATE_CORE_LOGGER_ERROR("Error: [{}] is not a valid uniform name for this program shader", name);
        else {
            // we pass we 1 because the shader uniform is not expected to be an array
            glUniform3fv(ret, 1, glm::value_ptr(value));
        }
    }

    auto OpenGLShader::SetUniformVec4(std::string_view name, const glm::vec4& value) -> void {
        Bind();
        auto ret{ glGetUniformLocation(getProgram(), name.data()) };

        if (ret == -1)
            KATE_CORE_LOGGER_ERROR("Error: [{}] is not a valid uniform name for this program shader", name);
        else {
            // we pass we 1 because the shader uniform is not expected to be an array
            glUniform4fv(ret, 1, glm::value_ptr(value));
        }
    }

    OpenGLShader::OpenGLShader(OpenGLShader &&other) noexcept
        :   m_Id{ other.getProgram() }
    {
        // Assign 0 so that it can be safely passed to glDeleteProgram()
        // when the destructor is called. We avoid deleting a valid program this way
        other.m_Id = 0;
    }

    auto OpenGLShader::operator=(OpenGLShader &&other) noexcept -> OpenGLShader & {
        m_Id = other.getProgram();
        other.m_Id = 0;
        return *this;
    }

    auto OpenGLShader::SetUniformVec2(std::string_view name, const glm::vec2 &vec) -> void {
        Bind();
        auto ret{ glGetUniformLocation(getProgram(), name.data()) };

        if (ret == -1)
            KATE_CORE_LOGGER_ERROR("Error: [{}] is not a valid uniform name for this program shader", name);
        else {
            // we pass we 1 because the shader uniform is not expected to be an array
            glUniform2fv(ret, 1, glm::value_ptr(vec));
        }
    }

    auto OpenGLShader::SetUniformMat3(std::string_view name, const glm::mat3& value) -> void {
        Bind();
        auto ret{ glGetUniformLocation(getProgram(), name.data()) };

        if (ret == -1)
            KATE_CORE_LOGGER_ERROR("Error: [{}] is not a valid uniform name for this program shader", name);
        else {
            /*
             * If transpose is GL_FALSE, each matrix is assumed to be supplied in column major order.
             * If transpose is GL_TRUE, each matrix is assumed to be supplied in row major order.
             * The count argument indicates the number of matrices to be passed. A count of 1
             * should be used if modifying the value of a single matrix, and a count greater
             * than 1 can be used to modify an array of matrices.
             * from: https://docs.gl/gl4/glUniform
             *
             * we pass GL_FALSE because glm::mat4 has each row stored contiguously in memory by default,
             * meaning the elements of the first row are stored first, followed by the
             * elements of the second row, and so on.
             * */
            glUniformMatrix3fv(ret, 1, GL_FALSE, glm::value_ptr(value));
        }
    }
}