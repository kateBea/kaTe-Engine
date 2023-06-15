//
// Created by kate on 6/4/23.
//

#ifndef KATE_ENGINE_VERTEX_BUFFER_HH
#define KATE_ENGINE_VERTEX_BUFFER_HH

#include <vector>
#include <string>
#include <string_view>
#include <utility>
#include <memory>

#include "Core/Assert.hh"
#include "Tools/Common.hh"

namespace kaTe {
    enum class ShaderDataType {
        NONE,
        FLOAT_TYPE,     // Represents a single float data type
        FLOAT2_TYPE,    // Represents a two float data type
        FLOAT3_TYPE,    // Represents a three float data type
        FLOAT4_TYPE,    // Represents a four float data type

        MAT3_TYPE,      // Represents 3x3 float matrix data type
        MAT4_TYPE,      // Represents 4x4 float matrix data type

        INT_TYPE,       // Represents a single int data type
        INT2_TYPE,      // Represents a two int data type
        INT3_TYPE,      // Represents a three int data type
        INT4_TYPE,      // Represents a four int data type
        BOOL_TYPE,      // Represents a single boolean data type
        COUNT,
    };

    /**
     * Describes an element contained within a buffer data
     * */
    class BufferElement {
    public:
        BufferElement(ShaderDataType type, std::string_view name, bool normalized = false)
            :   m_Name{ name }, m_Type{ type }, m_Size{ getSizeFromShaderType(type) }, m_Offset{ 0 }, m_Normalized{ normalized } {}

        auto getAttributeCount() const -> UInt32_T { return getComponentCount(m_Type); }
        auto getOpenGLAttributeDataType() const -> UInt32_T { return getOpenGLTypeFromShaderDataType(m_Type); }
        auto getAttributeSize() const -> UInt32_T { return m_Size; }

        /*  Getters */
        auto getName() const -> const std::string& { return m_Name; }
        auto getType() const -> ShaderDataType { return m_Type; }
        auto getSize() const -> UInt32_T { return m_Size; }
        auto getOffset() const -> UInt32_T { return m_Offset; }
        auto isNormalized() const  { return m_Normalized; }

        /*  Setters */
        auto setName(std::string_view name) -> void { m_Name = name; }
        auto setType(ShaderDataType type) -> void { m_Type = type; }
        auto setSize(UInt32_T size) -> void { m_Size = size; }
        auto setOffset(UInt32_T offset) -> void { m_Offset = offset; }
        auto setNormalized() -> void { m_Normalized = true; }
        auto unsetNormalized() -> void { m_Normalized = false; }

    private:
        // Size in bytes for integer
        static constexpr UInt32_T s_DefaultShaderIntSize{ 4 };
        // Size in bytes for float
        static constexpr UInt32_T s_DefaultShaderFloatSize{ 4 };

        std::string     m_Name{};
        ShaderDataType  m_Type{};
        UInt32_T        m_Size{};
        UInt32_T        m_Offset{};
        bool            m_Normalized;

        /**
         * Returns the size of the shader data type
         * @returns the size in bytes of the data type
         * */
        static constexpr auto getSizeFromShaderType(ShaderDataType type) -> UInt32_T {
            switch (type) {
                case ShaderDataType::FLOAT_TYPE:    return s_DefaultShaderFloatSize;
                case ShaderDataType::FLOAT2_TYPE:   return s_DefaultShaderFloatSize * 2;
                case ShaderDataType::FLOAT3_TYPE:   return s_DefaultShaderFloatSize * 3;
                case ShaderDataType::FLOAT4_TYPE:   return s_DefaultShaderFloatSize * 4;

                case ShaderDataType::MAT3_TYPE:     return s_DefaultShaderFloatSize * (3 * 3);
                case ShaderDataType::MAT4_TYPE:     return s_DefaultShaderFloatSize * (4 * 4);

                case ShaderDataType::INT_TYPE:      return s_DefaultShaderIntSize;
                case ShaderDataType::INT2_TYPE:     return s_DefaultShaderIntSize * 2;
                case ShaderDataType::INT3_TYPE:     return s_DefaultShaderIntSize * 3;
                case ShaderDataType::INT4_TYPE:     return s_DefaultShaderIntSize * 4;
                case ShaderDataType::BOOL_TYPE:     return 1;

                case ShaderDataType::NONE:
                case ShaderDataType::COUNT: [[fallthrough]];
                default: KT_ASSERT(false, "Invalid shader data type");
            }

            KT_ASSERT(false, "Invalid shader data type");
        }

        /**
         * Returns the number of components of the given type
         * @returns Count of elements of the data type
         * */
        static constexpr auto getComponentCount(ShaderDataType type) -> UInt32_T {
            switch(type) {
                case ShaderDataType::FLOAT_TYPE:    return 1;
                case ShaderDataType::FLOAT2_TYPE:   return 2;
                case ShaderDataType::FLOAT3_TYPE:   return 3;
                case ShaderDataType::FLOAT4_TYPE:   return 4;

                case ShaderDataType::MAT3_TYPE:     return 3 * 3;
                case ShaderDataType::MAT4_TYPE:     return 4 * 4;

                case ShaderDataType::INT_TYPE:      return 1;
                case ShaderDataType::INT2_TYPE:     return 2;
                case ShaderDataType::INT3_TYPE:     return 3;
                case ShaderDataType::INT4_TYPE:     return 4;
                case ShaderDataType::BOOL_TYPE:     return 1;

                case ShaderDataType::NONE:
                case ShaderDataType::COUNT: [[fallthrough]];
                default: KT_ASSERT(false, "Invalid shader data type");
            }

            KT_ASSERT(false, "Invalid shader data type");
        }

        static GLenum getOpenGLTypeFromShaderDataType(ShaderDataType type) {
            switch(type) {

                case ShaderDataType::FLOAT_TYPE:
                case ShaderDataType::FLOAT2_TYPE:
                case ShaderDataType::FLOAT3_TYPE:
                case ShaderDataType::FLOAT4_TYPE:

                case ShaderDataType::MAT3_TYPE:
                case ShaderDataType::MAT4_TYPE: return GL_FLOAT;

                case ShaderDataType::INT_TYPE:
                case ShaderDataType::INT2_TYPE:
                case ShaderDataType::INT3_TYPE:
                case ShaderDataType::INT4_TYPE:
                case ShaderDataType::BOOL_TYPE: return GL_BOOL;

                case ShaderDataType::NONE:
                case ShaderDataType::COUNT: [[fallthrough]];
                default: KT_ASSERT(false, "Invalid shader data type");
            }
        }
    };

    class BufferLayout {
    public:
        explicit BufferLayout(std::initializer_list<BufferElement>&& items)
            :    m_Stride{ 0 }, m_Items(std::forward<std::initializer_list<BufferElement>>(items))
        {
            computeOffsetAndStride();
        }

        auto getElements() const -> const std::vector<BufferElement>& { return m_Items; }
        auto getStride() const { return m_Stride; }

        auto begin() -> std::vector<BufferElement>::iterator { return m_Items.begin(); }
        auto end() -> std::vector<BufferElement>::iterator { return m_Items.end(); }

        auto begin() const -> std::vector<BufferElement>::const_iterator { return m_Items.begin(); }
        auto end() const -> std::vector<BufferElement>::const_iterator { return m_Items.end(); }

        auto rbegin() -> std::vector<BufferElement>::reverse_iterator { return m_Items.rbegin(); }
        auto rend() -> std::vector<BufferElement>::reverse_iterator { return m_Items.rend(); }

        auto rbegin() const -> std::vector<BufferElement>::const_reverse_iterator { return m_Items.rbegin(); }
        auto rend() const -> std::vector<BufferElement>::const_reverse_iterator { return m_Items.rend(); }
    private:
        // Helpers
        auto computeOffsetAndStride() -> void {
            UInt32_T offset{ 0 };
            for (auto& item : m_Items) {
                item.setOffset(offset);
                offset += item.getSize();
                m_Stride += item.getSize();
            }
        }

        UInt32_T m_Stride{};
        std::vector<BufferElement> m_Items{};
    };

    /**
     * General interface for Vertex buffers
     * */
    class VertexBuffer {
    public:
        VertexBuffer() = default;
        VertexBuffer(const BufferLayout& bufferLayout) ;
        virtual ~VertexBuffer() = default;

        KT_NODISCARD
        virtual auto GetID() const -> UInt32_T { return m_Id; }

        // Temporary for OpenGL VertexBuffer
        virtual auto BindBufferData() const -> void = 0;
        virtual auto UnbindBufferData() const -> void = 0;

        virtual auto SetBufferLayout(const BufferLayout& layout) -> void = 0;
        KT_NODISCARD
        virtual auto GetBufferLayout() const -> const BufferLayout& = 0;

        /**
         * Returns the total size in bytes of the contents of this Vertex buffer
         * @return total size in bytes of the vertices of this buffer
         * */
        [[nodiscard]]
        auto GetSize() const -> std::size_t { return m_Size; }

        auto IsEmpty() const -> bool { return m_Size == 0; }

        static auto CreateBuffer(const std::vector<float>& data) -> std::shared_ptr<VertexBuffer>;

    protected:
        UInt32_T m_Id{};
        UInt64_T m_Size{};
    };
}

#endif//KATE_ENGINE_VERTEX_BUFFER_HH
