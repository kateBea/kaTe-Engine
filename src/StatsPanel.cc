//
// Created by kate on 6/27/23.
//

#include <memory>

#include <imgui.h>

#include <Renderer/Renderer.hh>
#include <Renderer/Renderer2D.hh>
#include <Renderer/RenderCommand.hh>

#include "Editor/Panels/StatsPanel.hh"

namespace kaTe {

    StatsPanel::StatsPanel(const std::shared_ptr<StatsPanelData> &data, const Path_T &iconPath)
        :   Panel{ iconPath }, m_Visible{ true }, m_Hovered{ false }, m_Focused{ false }, m_Data{ data }
    {

    }

    auto StatsPanel::OnUpdate() -> void {
        if (m_Visible) {
            ImGui::Begin("Statistics");
            ImGui::Text("Draw calls count: %d", Renderer2D::QueryDrawCallsCount());
            ImGui::Text("Quad count: %d", Renderer2D::QueryQuadCount());
            ImGui::Text("Index count: %d", Renderer2D::QueryIndexCount());
            ImGui::Text("Vertex count: %d", Renderer2D::QueryVertexCount());
            ImGui::Text("Frame rate: %.1f", ImGui::GetIO().Framerate);

            ImGui::End();
        }
    }

    auto StatsPanel::OnEvent(Event &event) -> void {

    }
}