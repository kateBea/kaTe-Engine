#include <Core/Logger.hh>
#include <Core/Assert.hh>
#include <Renderer/OpenGL/OpenGLVertexArray.hh>

#include <Tools/Common.hh>

namespace kaTe {
    OpenGLVertexArray::OpenGLVertexArray(OpenGLVertexArray && other) noexcept
        :   m_Id{ other.getId() }, m_ValidId{ other.m_ValidId } { other.m_Id = 0; }

    auto OpenGLVertexArray::operator=(OpenGLVertexArray && other) noexcept -> OpenGLVertexArray & {
        m_Id = other.getId();
        other.m_Id = 0;
        return *this;
    }

    auto OpenGLVertexArray::useVertexBuffer(std::shared_ptr<VertexBuffer> buffer) -> void {
        KT_ASSERT(!buffer->isEmpty(), "Vertex Buffer is empty");
        bindVertexArray();
        buffer->bindBufferData();

        UInt32_T index{};
        for (const auto&i: buffer->getBufferLayout()) {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(index, i.getAttributeCount(), i.getOpenGLAttributeDataType(),
                                  !i.isNormalized() ? GL_FALSE : GL_TRUE, buffer->getBufferLayout().getStride(), (const void*)i.getOffset());
            ++index;
        }
    }


}