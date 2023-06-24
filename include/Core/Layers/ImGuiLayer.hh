/**
 * ImGuiLayer.hh
 * Created by kate on 5/28/23.
 * */

#ifndef KATE_ENGINE_IMGUI_LAYER_HH
#define KATE_ENGINE_IMGUI_LAYER_HH

// Third-Party Libraries
#include <imgui.h>

// Project Headers
#include <Core/Layers/Layer.hh>
#include <Core/Events/AppEvents.hh>
#include <Core/Events/Event.hh>
#include <Core/Events/KeyEvents.hh>
#include <Core/Events/MouseEvents.hh>

namespace kaTe {
    /**
     * ImGuiLayer interface for ImGui GUI elements
     * Uses OpenGL for rendering and GLFW for event handling
     * */
    class ImGuiLayer : public Layer {
    public:
        explicit ImGuiLayer() noexcept;
        ~ImGuiLayer() override;

        auto OnAttach() -> void override;
        auto OnDetach() -> void override;
        auto OnUpdate() -> void override;
        auto OnEvent(Event& event) -> void override;
        auto OnImGuiRender() -> void override;

        auto SetBlockEvents(bool value) -> void { m_BlockEvents = value; }

        auto BeginFrame() -> void;
        auto EndFrame() -> void;

    private:
        // Do not propagate events to this layer
        bool m_BlockEvents{ false };
        static auto SetupCustomImGuiStyle() -> void;

    };

}   // END NAMESPACE kaTe


#endif //KATE_ENGINE_IMGUI_LAYER_HH
