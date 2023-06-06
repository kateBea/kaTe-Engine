#include <Renderer/OpenGL/OpenGLVertexBuffer.hh>
#include <Renderer/Vertex.hh>

#include <Tools/Common.hh>

namespace kaTe {
    OpenGLVertexBuffer::OpenGLVertexBuffer(const std::vector<float>& vertices, GLenum usage) noexcept {
        glCreateBuffers(1, &m_Id);
        m_ValidId = m_Id != 0;

        load(vertices, usage);
    }


    OpenGLVertexBuffer::OpenGLVertexBuffer(OpenGLVertexBuffer && other) noexcept
        :   m_Layout{ std::move(other.m_Layout) }, m_ValidId{ other.m_ValidId }
    {
        m_Id = other.getRenderId();

        other.m_Id = 0;
        other.m_Size = 0;
    }

    auto OpenGLVertexBuffer::operator=(OpenGLVertexBuffer && other) noexcept -> OpenGLVertexBuffer & {
        m_Id = other.getRenderId();
        m_Size = other.getSize();

        other.m_Id = 0;
        other.m_Size = 0;
        return *this;
    }

    auto OpenGLVertexBuffer::load(const std::vector<float> &vertices, GLenum usage) -> void {
        if (!m_ValidId) {
            glCreateBuffers(1, &m_Id);
            m_ValidId = m_Id != 0;
        }

        if (!vertices.empty()) {
            bindBufferData();
            m_Size = vertices.size() * sizeof(float);
            glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(m_Size), vertices.data(), usage);
            unbindBufferData();
        }
    }
}