/**
* kateOpenGLVeo.hh
* Created by kate on 6/4/23.
* */

#ifndef KATE_ENGINE_VIB_HH
#define KATE_ENGINE_VIB_HH

// C++ Standard Library
#include <vector>
#include <cstdint>

// Third-Party Libraries
#include <GL/glew.h>

// Project Libraries
#include <Tools/Common.hh>
#include <Renderer/IndexBuffer.hh>

namespace kaTe {
    class OpenGLIndexBuffer : public IndexBuffer {
    public:
        explicit OpenGLIndexBuffer() = default;
        /**
         * Creates a new Vertex index buffer and initializes it with the data
         * from indices. If no data is provided simply creates a new index buffer object with a valid id
         * @param indices buffer containing all the indices values
         * */
        explicit OpenGLIndexBuffer(const std::vector<UInt32_T> &indices, GLenum usage = GL_STATIC_DRAW);

        /**
         * Mark this Vertex index buffer as current
         * */
        auto bindIndexBuffer() const -> void override { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, getRenderId()); }

        /**
         * Releases the currently bound Vertex element buffer.
         * NOTE: Buffer set to zero effectively unbinds any buffer object
         * previously bound, and restores client memory usage for that buffer object
         * target (if supported for that target). See: https://docs.gl/gl4/glBindBuffer
         * */
        auto unbindIndexBuffer() const -> void override { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

        /**
         * Move constructor
         * @param other moved from Vertex buffer
         * */
        OpenGLIndexBuffer(OpenGLIndexBuffer && other) noexcept;

        /**
         * Move assigment
         * @param other moved from Vertex buffer
         * */
        auto operator=(OpenGLIndexBuffer && other) noexcept -> OpenGLIndexBuffer&;

        auto load(const std::vector<UInt32_T> &indices, GLenum usage = GL_STATIC_DRAW) -> void;

        /**
         * Releases resources from this Vertex index buffer
         * */
        ~OpenGLIndexBuffer() { glDeleteBuffers(1, &m_Id); }

    private:
        // Forbidden operations
        OpenGLIndexBuffer(const OpenGLIndexBuffer &) = delete;
        auto operator=(const OpenGLIndexBuffer &) -> OpenGLIndexBuffer & = delete;

    private:
        /**
         * Tells whether this VAO holds a valid OpenGL shader program id.
         * For internal usage for now mainly
         * */
        bool m_ValidId{};
    };
}

#endif // KATE_ENGINE_VIB_HH