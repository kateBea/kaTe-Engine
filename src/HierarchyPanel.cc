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
        if (m_Visible) {
            ImGui::Begin("Hierarchy");
            if (auto ptr{ m_Context.lock() }) {
                auto view{ ptr->m_Registry.view<TagComponent>() };

                for (const auto& entity : view) {
                    DrawEntityNode(entity);
                    EntityPopupMenu(entity);
                }

                if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGui::IsWindowHovered())
                    m_ContextSelection = entt::null;

                BlankSpacePopupMenu();
            }
            else KATE_CORE_LOGGER_ERROR("Panel context has expired and no longer exists!");
            ImGui::End();
        }
    }

    auto HierarchyPanel::DrawEntityNode(const entt::entity target) -> void {
        if (auto ptr{ m_Context.lock() }) {
            TagComponent& tag{ ptr->m_Registry.get<TagComponent>(target) };
            bool thisEntityIsSelected{ target == m_ContextSelection};
            ImGuiTreeNodeFlags flags{  (thisEntityIsSelected ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow };

            bool expanded{ ImGui::TreeNodeEx((void*)target, flags, "%s", tag.GetTag().c_str()) };
            if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
                m_ContextSelection = target;

            if (expanded) {
                // Recursively expand
                ImGui::TreePop();
            }
        }

    }

    auto HierarchyPanel::EntityPopupMenu(const entt::entity &target) -> void {
        if (auto ptr{ m_Context.lock() }) {
            if (ImGui::BeginPopupContextItem(nullptr, ImGuiPopupFlags_MouseButtonRight)) {
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
                    if (ImGui::MenuItem("Destroy entity"))
                        ptr->m_Registry.destroy(target);
                    ImGui::EndMenu();
                }
                ImGui::EndPopup();
            }
        }
    }

    auto HierarchyPanel::BlankSpacePopupMenu() -> void {
        if (auto ptr{ m_Context.lock() }) {
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