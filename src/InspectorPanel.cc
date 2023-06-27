/**
 * InspectorPanel.cc
 * Created by kate on 6/25/23.
 * */

// C++ Standard Library
#include <array>
#include <utility>
#include <iterator>
#include <algorithm>

// Third-Party Libraries
#include <entt/entt.hpp>

#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <imgui_internal.h>

// Project Headers
#include <Core/Logger.hh>

#include <Scene/Component.hh>

#include <Editor/Panels/InspectorPanel.hh>

namespace kaTe {
    InspectorPanel::InspectorPanel(const std::shared_ptr<HierarchyPanel> &hierarchy, const Path_T& iconPath)
        :   Panel{ iconPath }, m_Hierarchy{ hierarchy }, m_Visible{ true }, m_Hovered{ false }, m_Focused{ false }
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
            bool removeSprite{ false };     // Tells whether we want to remove the Sprite or not
            bool removeTransform{ false };  // Tells whether we want to remove the transform or not
            bool removeCamera{ false };     // Tells whether we want to remove the camera or not
            bool removeScript{ false };     // Tells whether we want to remove the script or not

            if (auto ptr{ m_Hierarchy->m_Context.lock() })
                m_Hierarchy->m_ContextSelection.SetContext(ptr);

            // Tag Component
            if (m_Hierarchy->m_ContextSelection.HasComponent<TagComponent>()) {
                TagComponent& tag{ m_Hierarchy->m_ContextSelection.GetComponent<TagComponent>() };
                std::string value{ tag.GetTag() };
                static bool renderContextSelectionToScene{ true };
                char contextSelectionTagName[1024]{};
                std::copy(tag.GetTag().begin(), tag.GetTag().end(), contextSelectionTagName);

                static Entity currentSelectionBackup{};

                ImGui::Checkbox("##show", &renderContextSelectionToScene); ImGui::SameLine();
                //bool transformComponentNodeOpen{ ImGui::TreeNodeEx((void*) typeid(TagComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Tag") };

                if (ImGui::InputText("##Tag", contextSelectionTagName, std::size(contextSelectionTagName))) {
                    // backup the scene entity whose tag is being modified at the moment
                    currentSelectionBackup.m_EntityHandle = m_Hierarchy->m_ContextSelection.m_EntityHandle;
                    currentSelectionBackup.m_Scene = m_Hierarchy->m_ContextSelection.m_Scene;

                    tag.SetTag(contextSelectionTagName);
                }

            }

            // Transform Component
            if (m_Hierarchy->m_ContextSelection.HasComponent<TransformComponent>()) {

                bool transformComponentNodeOpen{ ImGui::TreeNodeEx((void*) typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform") };

                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4.0f, 4.0f });
                ImGui::SameLine(ImGui::GetWindowWidth() - 25.0f);
                if (ImGui::Button("+"))
                    ImGui::OpenPopup("PlusComponentSettingsButton");

                if (ImGui::BeginPopup("PlusComponentSettingsButton")) {
                    if (ImGui::MenuItem("Remove Component")) {
                        removeTransform = true;
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::EndPopup();
                }
                ImGui::PopStyleVar();

                if (transformComponentNodeOpen) {

                    TransformComponent& transform{ m_Hierarchy->m_ContextSelection.GetComponent<TransformComponent>() };
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

            // Camera Component
            if (m_Hierarchy->m_ContextSelection.HasComponent<CameraComponent>()) {
                static std::array<std::string, 2> cameraProjectionTypes{ "Orthographic", "Perspective" };
                CameraComponent& camera{ m_Hierarchy->m_ContextSelection.GetComponent<CameraComponent>() };
                auto cameraCurrentProjectionType{ camera.GetCameraPtr()->GetProjectionType() };
                auto currentProjectionTypeStr{ cameraProjectionTypes[cameraCurrentProjectionType] };

                bool cameraNodeOpen{ ImGui::TreeNodeEx((void*) typeid(SceneCamera).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera") };

                if (cameraNodeOpen) {

                    if (ImGui::BeginCombo("Projection", currentProjectionTypeStr.c_str())) {
                        UInt32_T projectionIndex{};
                        for (const auto& projectionType : cameraProjectionTypes) {
                            bool isSelected{ projectionType == cameraProjectionTypes[cameraCurrentProjectionType] };

                            if (ImGui::Selectable(projectionType.c_str(), isSelected)) {
                                currentProjectionTypeStr = projectionType;
                                camera.GetCameraPtr()->SetProjectionType((SceneCamera::ProjectionType) projectionIndex);
                            }

                            if (isSelected)
                                ImGui::SetItemDefaultFocus();

                            ++projectionIndex;
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
            if (m_Hierarchy->m_ContextSelection.HasComponent<SpriteRendererComponent>()) {
                bool spriteComponentNodeOpen{ ImGui::TreeNodeEx((void*) typeid(SpriteRendererComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Sprite") };

                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4.0f, 4.0f });
                ImGui::SameLine(ImGui::GetWindowWidth() - 25.0f);
                if (ImGui::Button("+"))
                    ImGui::OpenPopup("PlusSpriteComponentSettingsButton");

                if (ImGui::BeginPopup("PlusSpriteComponentSettingsButton")) {
                    if (ImGui::MenuItem("Remove Component")) {
                        removeSprite = true;
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::EndPopup();
                }
                ImGui::PopStyleVar();

                if (spriteComponentNodeOpen) {
                    SpriteRendererComponent& sprite{ m_Hierarchy->m_ContextSelection.GetComponent<SpriteRendererComponent>() };
                    glm::vec4 color{ sprite.GetColor() };
                    ImGui::ColorEdit4("Color", glm::value_ptr(color), ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_AlphaBar);
                    sprite.SetColor(color);
                    ImGui::TreePop();
                }
            }

            // Script component
            if (m_Hierarchy->m_ContextSelection.HasComponent<NativeScriptComponent>()) {
                if (ImGui::TreeNodeEx((void*) typeid(NativeScriptComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Script")) {

                    ImGui::TreePop();
                }
            }

            if (removeSprite)
                m_Hierarchy->m_ContextSelection.RemoveComponent<SpriteRendererComponent>();

            // cannot remove the transform component for now, only sprite

            if (m_Hierarchy->m_ContextSelection.IsValid()) {
                if (ImGui::Button("Add component"))
                    ImGui::OpenPopup("AddComponentButtonPopup");

                if (ImGui::BeginPopup("AddComponentButtonPopup")) {
                    if (ImGui::MenuItem("Sprite")) {
                        m_Hierarchy->m_ContextSelection.AddComponent<SpriteRendererComponent>();
                        ImGui::CloseCurrentPopup();
                    }
                    if (ImGui::MenuItem("Camera")) {
                        m_Hierarchy->m_ContextSelection.AddComponent<CameraComponent>(std::make_shared<SceneCamera>());
                        ImGui::CloseCurrentPopup();
                    }
                    if (ImGui::MenuItem("Script")) {
                        m_Hierarchy->m_ContextSelection.AddComponent<NativeScriptComponent>();
                        ImGui::CloseCurrentPopup();
                    }

                    ImGui::EndPopup();
                }

            }
            ImGui::End();
        }
    }

    auto InspectorPanel::OnEvent(Event &event) -> void {

    }
}