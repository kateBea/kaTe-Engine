//
// Created by kate on 6/25/23.
//

#include <utility>

#include <entt/entt.hpp>
#include <imgui.h>

#include "Editor/Panels/HierarchyPanel.hh"
#include "Scene/Entity.hh"
#include <Core/Logger.hh>
#include <Scene/Scene.hh>

namespace kaTe {
    auto HierarchyPanel::OnUpdate() -> void {
        if (IsVisible()) {
            // Show panel stuff if it is visible
            ImGui::Begin("Hierarchy");

            // Display entities names
            {
                if (auto ptr{ m_Context.lock() }) {
                    auto view{ ptr->m_Registry.view<TagComponent>() };
                    static entt::entity hoveredEntity{ entt::null };

                    for (const auto& entity : view) {
                        TagComponent& tag{ view.get<TagComponent>(entity) };

                        bool thisEntityIsSelected{ entity == m_ContextSelection};
                        ImGuiTreeNodeFlags flags{  (thisEntityIsSelected ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow };

                        bool expanded{ ImGui::TreeNodeEx((void*)entity, flags, "%s", tag.GetTag().c_str()) };
                        if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
                            m_ContextSelection = entity;

                        if (ImGui::IsItemHovered())
                            hoveredEntity = entity;

                        if (expanded) {
                            // Recursively expand
                            ImGui::TreePop();
                        }

                        // Deselect entity if needed
                        if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGui::IsWindowHovered())
                            m_ContextSelection = entt::null;

                    }


                    // Menu options for entities
                    // Just needs to be drawn once not per entity
                    if (ImGui::BeginPopupContextItem("##EntityMenuOptions", ImGuiPopupFlags_MouseButtonRight)) {
                        if (ImGui::BeginMenu("Add component")) {
                            if (ImGui::MenuItem("Tag")) {

                            }
                            if (ImGui::MenuItem("Transform")) {

                            }
                            if (ImGui::MenuItem("Sprite")) {

                            }
                            if (ImGui::MenuItem("Camera")) {

                            }
                            if (ImGui::MenuItem("Script")) {

                            }
                            ImGui::EndMenu();
                        }

                        if (ImGui::BeginMenu("Options")) {
                            if (ImGui::MenuItem("Destroy entity")) {
                                ptr->DestroyEntity(hoveredEntity);
                            }
                            ImGui::EndMenu();
                        }
                        ImGui::EndPopup();
                    }



                    // If we click on blank space in this panel
                    if (ImGui::BeginPopupContextWindow("##HierarchyMenuOptions", ImGuiPopupFlags_NoOpenOverItems | ImGuiPopupFlags_MouseButtonRight)) {
                        if (ImGui::BeginMenu("New")) {
                            if (ImGui::MenuItem("Create entity")) {
                                Scene::CreateEntity("Item", ptr);
                            }

                            ImGui::EndMenu();
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

    auto HierarchyPanel::OnEvent(Event& event) -> void {

    }

    HierarchyPanel::HierarchyPanel(const std::shared_ptr<Scene>& scene, const Path_T &iconPath)
        :   Panel{ iconPath }, m_Visible{ true }, m_Hovered{ false }, m_Focused{ false }
    {
        SetScene(scene);
    }

    auto HierarchyPanel::SetScene(const std::shared_ptr<Scene>& scene) -> void {
        m_Context = scene;
    }
}