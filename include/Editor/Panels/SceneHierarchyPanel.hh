//
// Created by kate on 6/25/23.
//

#ifndef KATE_ENGINE_SCENEHIERARCHYPANEL_HH
#define KATE_ENGINE_SCENEHIERARCHYPANEL_HH

#include <memory>

#include <entt/entt.hpp>

#include <Tools/Common.hh>
#include <Scene/Scene.hh>
#include <Editor/Panels/Panel.hh>

namespace kaTe {
    class SceneHierarchyPanel : public Panel {
    public:
        explicit SceneHierarchyPanel(const std::shared_ptr<Scene>& scene, const Path_T &iconPath = {});
        ~SceneHierarchyPanel() override = default;

        SceneHierarchyPanel(const SceneHierarchyPanel& other) = default;
        SceneHierarchyPanel(SceneHierarchyPanel&& other) = default;

        auto operator=(const SceneHierarchyPanel& other) -> SceneHierarchyPanel& = default;
        auto operator=(SceneHierarchyPanel&& other) -> SceneHierarchyPanel& = default;

        auto OnUpdate() -> void override;
        auto OnEvent(Event& event) -> void override;
        auto SetScene(const std::shared_ptr<Scene>& scene) -> void;
        auto MakeVisible(bool value) -> void override { m_Visible = value; }

        KT_NODISCARD auto IsHovered() const -> bool override { return m_Hovered; }
        KT_NODISCARD auto IsFocused() const -> bool override { return m_Focused; }
        KT_NODISCARD auto IsVisible() const -> bool override { return m_Visible; }
    private:
        friend class InspectorPanel;

    private:
        bool m_Visible{};
        bool m_Hovered;
        bool m_Focused;
        std::weak_ptr<Scene> m_Context{};

        // temporary, we may want to select more than one entity in our scene
        entt::entity m_ContextSelection{ entt::null };
    };
}

#endif//KATE_ENGINE_SCENEHIERARCHYPANEL_HH
