/**
 * kateOpenGLVao.hh
 * Created by kate on 6/4/23.
 * */

#ifndef KATE_ENGINE_VAO_HH
#define KATE_ENGINE_VAO_HH

// C++ Standard Library
#include <cstdint>
#include <array>
#include <memory>
#include <deque>


// Third-Party Libraries
#include <GL/glew.h>

// Project Libraries
#include "Renderer/Buffers/VertexBuffer.hh"
#include <Tools/Common.hh>

namespace kaTe {
    class OpenGLVertexArray {
    public:
        explicit OpenGLVertexArray() { glCreateVertexArrays(1, &m_Id); m_ValidId = m_Id != 0; }

        /**
         * Move constructor
         * @param other other Vao from which we move data
         * */
        OpenGLVertexArray(OpenGLVertexArray && other) noexcept;

        /**
         * Move constructor assignment
         * @param other other Vao from which we move data
         * @retunr *this
         * */
        auto operator=(OpenGLVertexArray && other) noexcept -> OpenGLVertexArray&;

        /**
         * Returns the identifier of this Vertex Array Object
         * @return identifier of this vao
         * */
        [[nodiscard]]
        auto getId() const -> UInt32_T { return m_Id; }

        /**
         * Mark this Vertex Array Object as current
         * */
        auto bindVertexArray() const -> void { glBindVertexArray(getId()); }

        /**
         * Unbinds the currently bound Vertex Array Object
         * */
        static auto unbindVertexArray() -> void { glBindVertexArray(0); }

        auto useVertexBuffer(std::shared_ptr<VertexBuffer> buffer) -> void;

        ~OpenGLVertexArray() { glDeleteVertexArrays(1, &m_Id); }
    private:
        // Forbidden operations
        OpenGLVertexArray(const OpenGLVertexArray & other) = delete;
        auto operator=(const OpenGLVertexArray & other) -> OpenGLVertexArray & = delete;
    private:
        UInt32_T m_Id{ 0 };

        /**
         * Tells whether this VAO holds a valid OpenGL shader program id.
         * For internal usage for now mainly
         * */
        bool m_ValidId{ false };
    };
}

#endif	// KATE_ENGINE_VAO_HH