//
// Created by kate on 6/27/23.
//

#include "imgui.h"

#include "Core/Application.hh"
#include "Editor/Panels/ScenePanel.hh"

namespace kaTe {

    ScenePanel::ScenePanel(const std::shared_ptr<ScenePanelData>& data, const Path_T& iconPath)
        :   Panel{ iconPath }, m_Visible{ true }, m_Hovered{ false }, m_Focused{ false }, m_Data{ data }
    {

    }

    auto ScenePanel::OnUpdate() -> void {
        if (m_Visible) {
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0,0});
            ImGui::Begin("Scene");
            m_Focused = ImGui::IsWindowFocused();
            m_Hovered = ImGui::IsWindowHovered();

            Application::GetPtr()->BlockImGuiLayerEvents(!m_Focused || !m_Hovered);

            auto viewPortDimensions{ ImGui::GetContentRegionAvail() };

            if (m_Data->ViewPortWidth != viewPortDimensions.x || m_Data->ViewPortHeight != viewPortDimensions.y) {
                m_Data->SceneFrameBuffer->Resize((UInt32_T)viewPortDimensions.x, (UInt32_T)viewPortDimensions.y);
                m_Data->ViewPortWidth = viewPortDimensions.x;
                m_Data->ViewPortHeight = viewPortDimensions.y;
                m_Data->Viewport->OnViewPortResize((UInt32_T)viewPortDimensions.x, (UInt32_T)viewPortDimensions.y);
            }

            ImTextureID textId{ reinterpret_cast<ImTextureID>(m_Data->SceneFrameBuffer->GetColorAttachmentId()) };
            float frameWidth{ static_cast<float>(m_Data->SceneFrameBuffer->GetFrameBufferProperties().width) };
            float frameHeight{ static_cast<float>(m_Data->SceneFrameBuffer->GetFrameBufferProperties().height) };
            ImGui::Image((ImTextureID)textId, ImVec2{frameWidth, frameHeight}, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
            ImGui::End();
            ImGui::PopStyleVar();
        }
    }

    auto ScenePanel::OnEvent(Event &event) -> void {

    }
}
