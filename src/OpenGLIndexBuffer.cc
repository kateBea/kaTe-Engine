#include <vector>

#include <Renderer/OpenGL/OpenGLIndexBuffer.hh>
#include <Tools/Common.hh>

namespace kaTe {
    OpenGLIndexBuffer::OpenGLIndexBuffer(const std::vector<UInt32_T>& indices, GLenum usage) {
        glCreateBuffers(1, &m_Id);
        m_ValidId = m_Id != 0;

        load(indices, usage);
    }

    auto OpenGLIndexBuffer::load(const std::vector<UInt32_T>& indices, GLenum usage) -> void {
        if (!m_ValidId) {
            glCreateBuffers(1, &m_Id);
            m_ValidId = m_Id != 0;
        }

        if (!indices.empty()) {
            bindIndexBuffer();
            m_Count = indices.size();
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(m_Count) * sizeof(UInt32_T), indices.data(), usage);
            unbindIndexBuffer();
        }
    }

    OpenGLIndexBuffer::OpenGLIndexBuffer(OpenGLIndexBuffer && other) noexcept {
        m_Id = other.getRenderId();
        m_Count = other.getCount();
        m_ValidId = other.m_ValidId;

        other.m_Id = 0;
        other.m_Count = 0;
    }

    auto OpenGLIndexBuffer::operator=(OpenGLIndexBuffer && other) noexcept -> OpenGLIndexBuffer & {
        m_Id = other.getRenderId();
        m_Count = other.getCount();
        m_ValidId = other.m_ValidId;

        other.m_Id = 0;
        other.m_Count = 0;
        return *this;
    }
}