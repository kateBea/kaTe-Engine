//
// Created by kate on 6/25/23.
//

#include <array>
#include <utility>

#include "entt/entt.hpp"

#include "glm/gtc/type_ptr.hpp"

#include "imgui.h"
#include "imgui_internal.h"

#include "Editor/Panels/InspectorPanel.hh"
#include "Editor/Panels/SceneHierarchyPanel.hh"

namespace kaTe {

    InspectorPanel::InspectorPanel(std::shared_ptr<SceneHierarchyPanel> hierarchy, const Path_T& iconPath)
        :   Panel{ iconPath }, m_Hierarchy{ std::move(hierarchy) }, m_Visible{ true }, m_Hovered{ false }, m_Focused{ false }
    {}

    static auto DrawVec3Transform(std::string_view label, glm::vec3& data, double resetValue = 0.0 , float columWidth = 100.0f) {
        // Group is part of a unique label
        ImGui::PushID(label.data());

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, columWidth);
        ImGui::Text("%s", label.data());
        ImGui::NextColumn();
        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 3.0f, 3.0f });
        float lineHeight{ GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f };
        ImVec2 buttonSize{ lineHeight + 3.0f, lineHeight };

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.1f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.1f, 1.0f });
        if (ImGui::Button("X", buttonSize))
            data.x = (float)resetValue;

        ImGui::SameLine();
        ImGui::DragFloat("##X", &data.x, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::PopStyleColor(3);
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.1f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.25f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.1f, 1.0f });
        if (ImGui::Button("Y", buttonSize))
            data.y = (float)resetValue;

        ImGui::SameLine();
        ImGui::DragFloat("##Y", &data.y, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::PopStyleColor(3);
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.25f, 0.3f, 0.9f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
        if (ImGui::Button("Z", buttonSize))
            data.z = (float)resetValue;

        ImGui::SameLine();
        ImGui::DragFloat("##Z", &data.z, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopStyleColor(3);
        ImGui::PopItemWidth();

        ImGui::PopStyleVar();
        ImGui::Columns(1);

        ImGui::PopID();
    }

    auto InspectorPanel::OnUpdate() -> void {
        if (IsVisible()) {
            ImGui::Begin("Inspector");

            if (auto ptr{ m_Hierarchy->m_Context.lock() }) {
                // Possibility to change tag name
                if (ptr->m_Registry.all_of<TagComponent>(m_Hierarchy->m_ContextSelection)) {
                    TagComponent& tag{ ptr->m_Registry.get<TagComponent>(m_Hierarchy->m_ContextSelection) };
                    std::string value{ tag.GetTag() };
                    // TODO: check imgui_demo for usage with std::string
                    // Tells wether we want to disable this component or not,
                    // NOTE: not remove it, just disable it so it no longers appears in the scene
                    // show is static just for testing purposes for now
                    static bool show{ true };

                    ImGui::Checkbox("##show", &show);
                    ImGui::SameLine();
                    if (ImGui::InputText("Tag", value.data(), value.size() + 1))
                        tag.SetTag(value);

                }

                // TransformComponent
                if (ptr->m_Registry.all_of<TransformComponent>(m_Hierarchy->m_ContextSelection)) {
                    if (ImGui::TreeNodeEx((void*) typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform")) {

                        TransformComponent& transform{ ptr->m_Registry.get<TransformComponent>(m_Hierarchy->m_ContextSelection) };
                        auto translation{ transform.GetTranslation() };
                        auto rotation{ transform.GetRotation() };
                        auto scale{ transform.GetScale() };

                        DrawVec3Transform("Translation", translation);
                        DrawVec3Transform("Rotation", rotation);
                        DrawVec3Transform("Scale", scale);

                        transform.SetTranslation(translation);
                        transform.SetRotation(rotation);
                        transform.SetScale(scale);

                        ImGui::TreePop();
                    }
                }

                // Camera
                if (ptr->m_Registry.all_of<CameraComponent>(m_Hierarchy->m_ContextSelection)) {
                    static std::array<std::string, 2> cameraProjectionTypes{ "Orthographic", "Perspective" };
                    CameraComponent& camera{ ptr->m_Registry.get<CameraComponent>(m_Hierarchy->m_ContextSelection) };
                    auto cameraCurrentProjectionType{ camera.GetCameraPtr()->GetProjectionType() };
                    auto currentProjectionTypeStr{ cameraProjectionTypes[cameraCurrentProjectionType] };

                    if (ImGui::TreeNodeEx((void*) typeid(SceneCamera).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera")) {

                        if (ImGui::BeginCombo("Projection", currentProjectionTypeStr.c_str())) {
                            UInt32_T index{};
                            for (const auto& projectionType : cameraProjectionTypes) {
                                bool isSelected{ projectionType == cameraProjectionTypes[cameraCurrentProjectionType] };

                                if (ImGui::Selectable(projectionType.c_str(), isSelected)) {
                                    currentProjectionTypeStr = projectionType;
                                    camera.GetCameraPtr()->SetProjectionType((SceneCamera::ProjectionType)index);
                                }

                                if (isSelected)
                                    ImGui::SetItemDefaultFocus();

                                ++index;
                            }

                            ImGui::EndCombo();
                        }

                        if (camera.GetCameraPtr()->GetProjectionType() == SceneCamera::ProjectionType::ORTHOGRAPHIC) {
                            float size{ (float)camera.GetCameraPtr()->GetOrthographicSize() };
                            if (ImGui::SliderFloat("Orthographic Size", &size, 2.0f, 10.0f))
                                camera.GetCameraPtr()->SetOrthographicSize(size);

                            float nearPlane{ (float)camera.GetCameraPtr()->GetOrthographicNearPlane() };
                            if (ImGui::SliderFloat("Orthographic Near", &nearPlane, -5.0, -1.0))
                                camera.GetCameraPtr()->SetOrthographicNearPlane(nearPlane);

                            float farPlane{ (float)camera.GetCameraPtr()->GetOrthographicFarPlane() };
                            if (ImGui::SliderFloat("Orthographic Far", &farPlane, 1.0, 5.0))
                                camera.GetCameraPtr()->SetOrthographicFarPlane(farPlane);

                            camera.GetCameraPtr()->SetOrthographic(nearPlane, farPlane, size);
                        }

                        if (camera.GetCameraPtr()->GetProjectionType() == SceneCamera::ProjectionType::PERSPECTIVE) {
                            float fov{ (float)camera.GetCameraPtr()->GetPerspectiveFOV() };
                            if (ImGui::SliderFloat("Perspective FOV", &fov, 45.0f, 90.0f))
                                camera.GetCameraPtr()->SetPerspectiveFOV(fov);

                            float nearPlane{ (float)camera.GetCameraPtr()->GetPerspectiveNearPlane() };
                            if (ImGui::SliderFloat("Perspective Near", &nearPlane, 0.001f, 1.0))
                                camera.GetCameraPtr()->SetPerspectiveNearPlane(nearPlane);

                            float farPlane{ (float)camera.GetCameraPtr()->GetPerspectiveFarPlane() };
                            if (ImGui::SliderFloat("Perspective Far", &farPlane, 100.0f, 10000.0f))
                                camera.GetCameraPtr()->SetPerspectiveFarPlane(farPlane);

                            camera.GetCameraPtr()->SetPerspective(nearPlane, farPlane, fov);
                        }

                        ImGui::TreePop();
                    }
                }

                // Sprite
                if (ptr->m_Registry.all_of<SpriteRendererComponent>(m_Hierarchy->m_ContextSelection)) {
                    if (ImGui::TreeNodeEx((void*) typeid(SpriteRendererComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Sprite")) {
                        SpriteRendererComponent& sprite{ ptr->m_Registry.get<SpriteRendererComponent>(m_Hierarchy->m_ContextSelection) };
                        glm::vec4 color{ sprite.GetColor() };
                        ImGui::ColorEdit4("Color", glm::value_ptr(color), ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_AlphaBar);
                        sprite.SetColor(color);
                        ImGui::TreePop();
                    }
                }

            }

            ImGui::End();
        }
    }

    auto InspectorPanel::OnEvent(Event &event) -> void {

    }
}