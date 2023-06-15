/**
 * ImGuiLayer.hh
 * Created by kate on 5/28/23.
 * */

#ifndef KATE_ENGINE_IMGUI_LAYER_HH
#define KATE_ENGINE_IMGUI_LAYER_HH

// Third-Party Libraries
#include "imgui.h"

// Project Headers
#include "Core/Layers/Layer.hh"

#include "Core/Events/AppEvents.hh"
#include "Core/Events/Event.hh"
#include "Core/Events/KeyEvents.hh"
#include "Core/Events/MouseEvents.hh"

namespace kaTe {
    /**
     * ImGuiLayer interface for ImGui GUI elements
     * Uses OpenGL for rendering and GLFW for event handling
     * */
    class ImGuiLayer : public Layer {
    public:
        explicit ImGuiLayer() noexcept;
        ~ImGuiLayer();

        auto onAttach() -> void override;
        auto onDetach() -> void override;
        auto onUpdate() -> void override;
        auto onImGuiRender() -> void override;

        static auto beginFrame() -> void;
        static auto endFrame() -> void;

    private:
        static auto setupCustomImGuiStyle() -> void;

    };

}   // END NAMESPACE kaTe


#endif //KATE_ENGINE_IMGUI_LAYER_HH
