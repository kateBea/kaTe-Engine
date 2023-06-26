//
// Created by kate on 6/25/23.
//

#include <utility>

#include <entt/entt.hpp>
#include <imgui.h>

#include "Editor/Panels/SceneHierarchyPanel.hh"
#include "Scene/Entity.hh"
#include <Core/Logger.hh>
#include <Scene/Scene.hh>

namespace kaTe {
    auto SceneHierarchyPanel::OnUpdate() -> void {
        if (IsVisible()) {
            // Show panel stuff if it is visible
            ImGui::Begin("Hierarchy");

            // Display entities names
            {
                if (auto ptr{ m_Context.lock() }) {
                    auto view{ ptr->m_Registry.view<TagComponent>() };

                    for (const auto& entity : view) {
                        TagComponent& tag{ view.get<TagComponent>(entity) };

                        bool thisEntityIsSelected{ entity == m_ContextSelection};
                        ImGuiTreeNodeFlags flags{  (thisEntityIsSelected ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow };

                        bool expanded{ ImGui::TreeNodeEx((void*)entity, flags, "%s", tag.GetTag().c_str()) };
                        if (ImGui::IsItemClicked(/* when we left click it*/)) {
                            m_ContextSelection = entity;
                        }

                        // If we click on blank space in this entity. Doesnt work
                        if (ImGui::BeginPopupContextItem(nullptr)) {
                            if (ImGui::MenuItem("Destroy entity")) {
                                ptr->DestroyEntity(entity);
                            }
                            ImGui::EndPopup();
                        }

                        if (expanded) {
                            // Recursively expand
                            ImGui::TreePop();
                        }

                        // Deselect entity if needed
                        if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGui::IsWindowHovered())
                            m_ContextSelection = entt::null;

                    }

                    // If we click on blank space in this panel
                    if (ImGui::BeginPopupContextWindow(nullptr)) {
                        if (ImGui::MenuItem("Create entity")) {
                            Scene::CreateEntity("Item", ptr);
                        }
                        ImGui::EndPopup();
                    }
                }
                else
                    KATE_CORE_LOGGER_ERROR("Panel context has expired and no longer exists!");

            }

            ImGui::End();
        }
    }

    auto SceneHierarchyPanel::OnEvent(Event &event) -> void {

    }

    SceneHierarchyPanel::SceneHierarchyPanel(std::shared_ptr<Scene> scene, const Path_T &iconPath)
        :   Panel{ iconPath }, m_Visible{ true }, m_Hovered{ false }, m_Focused{ false }
    {
        SetScene(std::move(scene));
    }
    auto SceneHierarchyPanel::SetScene(std::shared_ptr<Scene> scene) -> void {
        m_Context = scene;
    }
}