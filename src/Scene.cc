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
        // Update scripts
        {

        }

        std::shared_ptr<Camera> mainCam{};
        bool sceneHasMainCam{ false };
        {
            auto view{ m_Registry.view<TransformComponent, CameraComponent>() };

            for (const auto& entity : view) {
                TransformComponent& transform{ view.get<TransformComponent>(entity) };
                CameraComponent& camera{ view.get<CameraComponent>(entity) };
                sceneHasMainCam = camera.IsMainCamera();

                if (sceneHasMainCam) {
                    mainCam = std::move(camera.GetCameraPtr());

                    // The camera's position, etc depends on its transform component
                    mainCam->SetTransform(transform.GetTransform());
                    break;
                }
            }
        }

        if (sceneHasMainCam) {
            // Render stuff if the scene has a camera

            Renderer2D::BeginScene(mainCam);

            auto view{ m_Registry.view<TagComponent, TransformComponent, SpriteRendererComponent>() };

            for (const auto& entity : view) {
                TagComponent& tag{ view.get<TagComponent>(entity) };
                TransformComponent& transform{ view.get<TransformComponent>(entity) };
                SpriteRendererComponent& sprite{ view.get<SpriteRendererComponent>(entity) };

                Renderer2D::DrawQuad(transform.GetTransform(), sprite.GetColor());
            }

            Renderer2D::EndScene();
        }
    }

    auto Scene::CreateEntity(std::string_view entityNameTag, std::shared_ptr<Scene> scene) -> Entity {
        Entity result{ std::move(scene) };

        // By default, all entities will have a transform component and a tag
        result.AddComponent<TagComponent>(entityNameTag);
        result.AddComponent<TransformComponent>(glm::vec3{ 0.0, 0.0, 0.0 },  glm::vec3{ 1.0f, 1.0f, 0.0f }, glm::vec3{ 0.0f, 0.0f, 0.0f });

        return result;
    }

    auto Scene::DestroyEntity(Entity entity) -> void {
        // Cast necessary to call appropriate overload
        DestroyEntity(static_cast<entt::entity>(entity));
    }

    auto Scene::DestroyEntity(entt::entity entity) -> void {
        m_Registry.destroy(entity);
    }

    auto Scene::OnViewPortResize(UInt32_T width, UInt32_T height) -> void {
        m_ViewportWidth = width;
        m_ViewportHeight = height;

        // Resize non-fixed aspect ratio cameras
        auto view{ m_Registry.view<TransformComponent, CameraComponent>() };

        for (const auto& entity : view) {
            TransformComponent& transform{ view.get<TransformComponent>(entity) };
            CameraComponent& camera{ view.get<CameraComponent>(entity) };

            if (!camera.IsAspectRatioFixed())
                camera.GetCameraPtr()->SetViewportSize(width, height);

        }
    }
}