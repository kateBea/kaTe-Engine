/**
* kateOpenGLVertexBuffer.hh
* Created by kate on 6/4/23.
* */


#ifndef KATE_ENGINE_OPENGL_VERTEX_BUFFER_HH
#define KATE_ENGINE_OPENGL_VERTEX_BUFFER_HH

// C++ Standard Library
#include <vector>
#include <cstdint>

// Third-Party Libraries
#include <GL/glew.h>

// Project Libraries
#include "Renderer/Buffers/VertexBuffer.hh"
#include <Tools/Common.hh>

namespace kaTe {
    class OpenGLVertexBuffer : public VertexBuffer {
    public:
        explicit OpenGLVertexBuffer() = default;

        /**
         * Creates a new Vertex buffer and initializes it with the data
         * from indices. If no data is provided simply creates a new index
         * buffer object with a valid id
         * @param vertices buffer containing all the vertices
         * */
        explicit OpenGLVertexBuffer(const std::vector<float>& vertices, GLenum usage = GL_STATIC_DRAW) noexcept;

        /**
         * Move constructor. If this operation is successful
         * other becomes invalid and using it may result in undefined behaviour
         * @param other moved from Vertex buffer
         * */
        OpenGLVertexBuffer(OpenGLVertexBuffer && other) noexcept;

        /**
         * Move constructor assignment. If this operation is successful
         * other becomes invalid and using it may result in undefined behaviour
         * @param other moved from Vertex buffer
         * */
        OpenGLVertexBuffer & operator=(OpenGLVertexBuffer && other) noexcept;

        /**
         * Mark this Vertex buffer as current
         * */
        auto Bind() const -> void override { glBindBuffer(GL_ARRAY_BUFFER, GetID()); }

        /**
         * Releases the currently bound Vertex buffer object.
         * NOTE: Buffer set to zero effectively unbinds any buffer object
         * previously bound, and restores client memory usage for that buffer object
         * target (if supported for that target). See: https://docs.gl/gl4/glBindBuffer
         * */
        auto Unbind() const -> void override { glBindBuffer(GL_ARRAY_BUFFER, 0); }

        /**
         * Returns the total count of vertices of this Vertex buffer
         * @return count of vertices
         * */
        KT_NODISCARD auto GetCount() const -> std::size_t { return m_Size / sizeof(float); /* Assumes float for attribute components */ }

        auto Upload(const std::vector<float>& vertices, GLenum usage = GL_STATIC_DRAW) -> void;

        auto SetBufferLayout(const BufferLayout& layout) -> void override { m_Layout = layout; }

        KT_NODISCARD
        auto GetBufferLayout() const -> const BufferLayout& override { return m_Layout; }
        /**
         * Releases resources from this Vertex buffer
         * */
        ~OpenGLVertexBuffer() override { glDeleteBuffers(1, &m_Id); }
    public:
        // Forbidden operations
        OpenGLVertexBuffer(const OpenGLVertexBuffer & other) = delete;
        auto operator=(const OpenGLVertexBuffer & other) -> OpenGLVertexBuffer & = delete;
    private:
        BufferLayout m_Layout{};

        /**
         * Tells whether this VAO holds a valid OpenGL shader program id.
         * For internal usage for now mainly
         * */
        bool m_ValidId{};
    };
}

#endif	// KATE_ENGINE_OPENGL_VERTEX_BUFFER_HH