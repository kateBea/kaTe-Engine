//
// Created by kate on 6/24/23.
//

#ifndef KATE_ENGINE_COMPONENT_HH
#define KATE_ENGINE_COMPONENT_HH

#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Tools/Common.hh>

namespace kaTe {
    class TagComponent {
    public:
        explicit TagComponent() = default;

        explicit TagComponent(std::string_view tag) {
            m_Tag = tag;
        }

        TagComponent(const TagComponent& other) = default;
        TagComponent(TagComponent&& other) = default;

        auto operator=(const TagComponent& other) -> TagComponent& = default;
        auto operator=(TagComponent&& other) -> TagComponent& = default;

        KT_NODISCARD auto GetTag() const -> const std::string& { return m_Tag; }
    private:
        std::string m_Tag{};
    };

    class TransformComponent {
    public:
        explicit TransformComponent() = default;

        explicit TransformComponent(const glm::mat4& data) { m_Transform = data; }

        TransformComponent(const glm::vec3& position, const glm::vec3& size, const glm::vec3& angles = glm::vec3(0.0f)) {
            ComputeTransform(position, size, angles);
        }

        TransformComponent(const TransformComponent& other) = default;
        TransformComponent(TransformComponent&& other) = default;

        auto operator=(const TransformComponent& other) -> TransformComponent& = default;
        auto operator=(TransformComponent&& other) -> TransformComponent& = default;

        KT_NODISCARD auto GetTranslation() const -> const glm::vec3& { return m_Translation; }
        KT_NODISCARD auto GetRotation() const -> const glm::vec3& { return m_Rotation; }
        KT_NODISCARD auto GetScale() const -> const glm::vec3& { return m_Scale; }
        KT_NODISCARD auto GetTransform() const -> const glm::mat4& { return m_Transform; }

        // angles specifies the rotation angle in degrees of each axis
        auto ComputeTransform(const glm::vec3& position, const glm::vec3& size, const glm::vec3& angles = glm::vec3(0.0f)) -> void {
            m_Translation = position;
            m_Scale = size;
            m_Rotation = angles;

            // Matrix transformations
            glm::mat4 scale{ glm::scale(identMat, size) };

            glm::mat4 rotationX{ glm::rotate(identMat, (float)glm::radians(angles[0]), xAxis) };
            glm::mat4 rotationY{ glm::rotate(rotationX, (float)glm::radians(angles[1]), yAxis) };
            glm::mat4 rotation{ glm::rotate(rotationY, (float)glm::radians(angles[2]), zAxis) };

            m_Transform =  glm::translate(identMat, position) * scale * rotation;
        }

        auto RecomputeTransform() -> void {
            // Matrix transformations
            glm::mat4 scale{ glm::scale(identMat, m_Scale) };

            glm::mat4 rotationX{ glm::rotate(identMat, (float)glm::radians(m_Rotation[0]), xAxis) };
            glm::mat4 rotationY{ glm::rotate(rotationX, (float)glm::radians(m_Rotation[1]), yAxis) };
            glm::mat4 rotation{ glm::rotate(rotationY, (float)glm::radians(m_Rotation[2]), zAxis) };

            m_Transform =  glm::translate(identMat, m_Translation) * scale * rotation;
        }

        auto SetTranslation(const glm::vec3& value) -> void { m_Translation = value; RecomputeTransform(); }
        auto SetRotation(const glm::vec3& value) -> void { m_Translation = value; RecomputeTransform(); }
        auto SetScale(const glm::vec3& value) -> void { m_Translation = value; RecomputeTransform(); }
        auto SetTransform(const glm::mat4& value) -> void { m_Transform = value; }

        ~TransformComponent() = default;
    private:
        // Constants
        static constexpr glm::vec3 xAxis{ 1.0f, 0.0f, 0.0f };
        static constexpr glm::vec3 yAxis{ 0.0f, 1.0f, 0.0f };
        static constexpr glm::vec3 zAxis{ 0.0f, 0.0f, 1.0f };
        static constexpr glm::mat4 identMat{ glm::mat4(1.0) };
    private:
        glm::vec3 m_Translation{};
        glm::vec3 m_Rotation{};
        glm::vec3 m_Scale{};

        glm::mat4 m_Transform{};
    };

    class SpriteRendererComponent {
    public:
        explicit SpriteRendererComponent() = default;

        explicit SpriteRendererComponent(const glm::vec4& color) {
            m_Color = color;
        }

        SpriteRendererComponent(const SpriteRendererComponent& other) = default;
        SpriteRendererComponent(SpriteRendererComponent&& other) = default;

        auto operator=(const SpriteRendererComponent& other) -> SpriteRendererComponent& = default;
        auto operator=(SpriteRendererComponent&& other) -> SpriteRendererComponent& = default;

        KT_NODISCARD auto GetColor() const -> const glm::vec4& { return m_Color; }

        auto SetColor(const glm::vec4& value) -> void { m_Color = value; }

        ~SpriteRendererComponent() = default;

    private:
        glm::vec4 m_Color{};
    };

    class CameraComponent {
    public:

    private:

    };
}
#endif//KATE_ENGINE_COMPONENT_HH
