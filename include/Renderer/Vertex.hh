//
// Created by kate on 6/4/23.
//

#ifndef KATE_ENGINE_VERTEX_HH
#define KATE_ENGINE_VERTEX_HH

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#include <Tools/Common.hh>

namespace kaTe {
    struct Vertex {
        /**
         * Indicates a vertex buffer attribute
         * */
        enum class Attribute {
            NONE,
            POSITION_ATTRIBUTE,
            NORMAL_ATTRIBUTE,
            TEXTURE_ATTRIBUTE,
            COUNT,
        };

        glm::vec3 m_Pos{};
        glm::vec3 m_Norm{};
        glm::vec2 m_Texture{};

        Vertex(const glm::vec3& pos, const glm::vec3& norm = glm::vec3(), const glm::vec2& texture = glm::vec2())
            :   m_Pos{ pos }, m_Norm{ norm }, m_Texture{ texture }
        {}

        Vertex() = default;
        ~Vertex() = default;

        /**
         * Return reference to the Vertex positions
         * */
        auto getPositions() -> glm::vec3& { return m_Pos; }

        /**
         * Return reference to the Vertex positions
         * */
        [[nodiscard]]
        auto getPositions() const -> const glm::vec3& { return m_Pos; }

        /**
         * Return reference to the Vertex normals
         * */
        [[nodiscard]]
        auto getNormals() -> glm::vec3& { return m_Norm; }

        /**
         * Return reference to the Vertex normals
         * */
        [[nodiscard]]
        auto getNormals() const -> const glm::vec3& { return m_Norm; }

        /**
         * Return reference to the Vertex texture coordinates
         * */
        [[nodiscard]]
        auto getTextures() -> glm::vec2& { return m_Texture; }

        /**
         * Return reference to the Vertex textures
         * */
        [[nodiscard]]
        auto getTextures() const -> const glm::vec2& { return m_Texture; }


        /**
         * Set the Vertex positions to the given data
         * @param data new vertex positions for this Vertex
         * */
        auto setPositions(const glm::vec3& data) -> void { m_Pos = data; }

        /**
         * Set the Vertex normals to the given data
         * @param data new vertex normal for this Vertex
         * */
        auto setNormals(const glm::vec3& data) -> void { m_Norm = data; }

        /**
         * Set the Vertex textures to the given data
         * @param data new vertex texture coordinates for this Vertex
         * */
        auto setTextures(const glm::vec2& data) -> void { m_Texture = data; }

        /**
         * Returns the offset of the given attribute type in the kT::Vertex structure
         * @param attribute vertex attribute
         * @return byte offset from this attribute within the kT::Vertex structure
         * */
        static constexpr auto getAttributeOffset(Attribute attribute) -> Int32_T {
            switch (attribute) {
                case Attribute::POSITION_ATTRIBUTE: return offsetof(Vertex, m_Pos);
                case Attribute::NORMAL_ATTRIBUTE: return offsetof(Vertex, m_Norm);
                case Attribute::TEXTURE_ATTRIBUTE: return offsetof(Vertex, m_Texture);
                default: return -1;
            }
        }
    };
}
#endif//KATE_ENGINE_VERTEX_HH
