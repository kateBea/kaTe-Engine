/**
 * Shader.hh
 * Created by kate on 6/4/23.
 */

#ifndef KATE_ENGINE_SHADER_HH
#define KATE_ENGINE_SHADER_HH

// C++ Standard Library
#include <string_view>
#include <filesystem>

// Third-Party Libraries
#include "GL/glew.h"

#include <glm/glm.hpp>

// Project headers
#include "Tools/Common.hh"

namespace kaTe {
    class Shader {
    public:
        /**
         * Default initialization for Shader. DOES NOT Create a valid shader program
         * */
        explicit Shader() = default;

        /**
         * Move constructor
         * */
         Shader(Shader&& other) noexcept;

        /**
         * Move assignment
         * @return *this
         * */
        Shader& operator=(Shader&& other) noexcept;

        /**
         * Construct Shader program from path to Vertex Shader source file directory
         * and pixel Shader source file directory
         * @param vertexSourceDir directory to the Vertex Shader source file
         * @param fragmentSourceDir directory to the pixel Shader source file
         * */
        Shader(const std::filesystem::path& vertexSourceDir, const std::filesystem::path& fragmentSourceDir);

        /**
         * Loads the shaders specified from paths
         * @param vShaderPath path to vertex shader path
         * @param fShaderPath path to pixel/fragment shader path
         * @throws std::runtime_error exception if any of the shader files could not be opened
         * */
        auto load(const std::filesystem::path& vShaderPath, const std::filesystem::path& fShaderPath) -> void;

        /**
         * Use this Shader program
         * */
        auto useProgram() const -> void { glUseProgram(m_Id); }

        /**
         * Get Shader program ID
         * @return Shader program ID
         * */
        [[nodiscard]]
        auto getProgram() const -> UInt32_T { return m_Id; }

        /**
         * Sets the given boolean value to the uniform identified by "name",
         * it has no effect if this Shader has no uniform with given name. This function
         * ensures this shader is being used before passing the data to the shader uniform, so
         * a previous call to Shader::use() is unnecessary
         * @param name name of the uniform
         * @param value value to be set
         * */
        auto setUniformBool(std::string_view name, bool value) const -> void;

        /**
         * Sets the given integer value to the uniform identified by "name",
         * it has no effect if this Shader has no uniform with given identifier. This function
         * ensures this shader is being used before passing the data to the shader uniform, so
         * a previous call to Shader::use() is unnecessary
         * @param name name of the uniform.
         * @param value value to be set
         * */
        auto setUniformInt(std::string_view name, Int32_T value) const -> void;

        /**
         * Sets the given floating value to the uniform identified by "name",
         * it has no effect if this Shader has no uniform with given identifier. This function
         * ensures this shader is being used before passing the data to the shader uniform, so
         * a previous call to Shader::use() is unnecessary
         * @param name name of the uniform
         * @param value value to be set
         * */
        auto setUniformFloat(std::string_view name, float value) const -> void;

        /**
         * Sets the given matrix to the uniform matrix specified by the name. This function
         * ensures this shader is being used before passing the data to the shader uniform, so
         * a previous call to Shader::use() is unnecessary
         * @param name name of the uniform
         * @param mat value for the uniform
         * */
        auto setUniformMat4(std::string_view name, const glm::mat4& mat) const -> void;

        /**
         * Sets the given 3D vector to the uniform specified by the name. This function
         * ensures this shader is being used before passing the data to the shader uniform, so
         * a previous call to Shader::use() is unnecessary
         * @param name name of the uniform
         * @param vec value for the uniform
         * */
        auto setUniformVec3(std::string_view name, const glm::vec3& vec) const -> void;

        /**
         * Sets the given 4D vector to the uniform specified by the name. This function
         * ensures this shader is being used before passing the data to the shader uniform, so
         * a previous call to Shader::use() is unnecessary
         * @param name name of the uniform
         * @param vec value for the uniform
         * */
        auto setUniformVec4(std::string_view name, const glm::vec4& vec) const -> void;

        /**
         * Perform cleanup
         * */
        ~Shader() { glDeleteProgram(getProgram()); }
    private:
        // Forbidden operations
        Shader(const Shader&) = delete;
        Shader& operator=(const Shader&) = delete;

    private:
        /**
         * Identifies a type of shader
         * */
        enum class ShaderType {
            NONE,
            VERTEX_SHADER_TYPE,
            FRAGMENT_SHADER_TYPE,
            COUNT,
        };

        /**
         * Returns an error message indicating the type of shader
         * This is a helper function for showing compilation status on Shader::compile()
         * @param type type of shader
         * */
        constexpr auto getShaderTypeStr(ShaderType type) const -> std::string_view {
            switch (type) {
                case ShaderType::VERTEX_SHADER_TYPE: return "VERTEX_SHADER_TYPE";
                case ShaderType::FRAGMENT_SHADER_TYPE:  return "FRAGMENT_SHADER_TYPE";
                default: return "Unknown type of shader";
            }
        }

        /**
         * Compiles the given shader and returns its corresponding identifier
         * @param content file contents of the shader
         * @param shaderType type of shader to be compiled
         * @return identifier of the compiled shader, 0 if there was an error
         * */
        static auto compile(const char* content, GLenum shaderType) -> UInt32_T;

        /**
         * Compiles and links the given shaders to this program shader.
         * @param vShader file contents of the vertex shader
         * @param fShader file contents of the fragment shader
         * */
        auto build(const char* vShader, const char* fShader) const -> void;

        /**
         * Helper function to retrieve Shader status
         * @param objectId identifier of the shader object
         * @param str error message indicating the type of shader
         * @param status
         * */
        auto showShaderStatus(UInt32_T objectId, ShaderType type, GLenum status) const -> void;

        /**
         * Helper function to retrieve program status
         * @param objectId identifier of the program object
         * @param str error message indicating the type of shader
         * @param status
         * */
        auto showProgramStatus(GLenum status) const -> void;

        /**
         * Identifier of this Shader program
         * */
        UInt32_T m_Id{};

        /**
         * Tells whether this shader holds a valid OpenGL shader program id.
         * For internal usage for now mainly
         * */
         bool m_ValidId{};
    };
}

#endif // END SHADER_HH