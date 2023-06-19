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
        KT_ASSERT(!buffer->IsEmpty(), "Vertex Buffer is empty");
        bindVertexArray();
        buffer->Bind();

        UInt32_T index{};
        for (const auto&i: buffer->GetBufferLayout()) {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(index, i.GetAttributeCount(), i.GetOpenGLAttributeDataType(),
                                  !i.IsNormalized() ? GL_FALSE : GL_TRUE, buffer->GetBufferLayout().GetStride(), (const void*) i.GetOffset());
            ++index;
        }
    }


}