//
// Created by kate on 6/24/23.
//

#include <entt/entt.hpp>

#include <Renderer/Renderer2D.hh>

#include "Scene/Scene.hh"
#include <Scene/Component.hh>
#include <Scene/Entity.hh>
#include <utility>

namespace kaTe {

    Scene::Scene() {
        entt::entity entity{ m_Registry.create() };
        m_Registry.emplace<TransformComponent>(entity, TransformComponent());
    }

    auto Scene::OnUpdate() -> void {
        auto view{ m_Registry.view<TransformComponent, SpriteRendererComponent>() };

        for (const auto& entity : view) {
            TransformComponent& transform{ view.get<TransformComponent>(entity) };
            SpriteRendererComponent& sprite{ view.get<SpriteRendererComponent>(entity) };

            Renderer2D::DrawQuad(transform.GetTransform(), sprite.GetColor());
        }
    }

    auto Scene::CreateEntity(std::string_view entityNameTag, std::shared_ptr<Scene> scene) -> Entity {
        Entity result{ std::move(scene) };

        // By default, all entities will have a transform component and a tag
        result.AddComponent<TransformComponent>(glm::vec3{ 0.0, 1.0, 0.0 },  glm::vec3{ 1.0f, 1.0f, 0.0f }, glm::vec3{ 0.0f, 0.0f, 0.0f });
        result.AddComponent<TagComponent>(entityNameTag);

        return result;
    }
}