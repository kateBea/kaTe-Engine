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

#include <Core/Events/Event.hh>
#include <Core/Events/AppEvents.hh>
#include <Core/Events/MouseEvents.hh>
#include <Core/Events/KeyEvents.hh>

namespace kT {
    class ImGuiLayer : public Layer {
    public:
        explicit ImGuiLayer() noexcept;
        ~ImGuiLayer();

        auto onAttach() -> void override;
        auto onDetach() -> void override;
        auto onUpdate() -> void override;
        auto onEvent(Event& event) -> void override;

    private:
        /*
         * EVENT HANDLERS
         * */
        auto onMouseMovedEvent(MouseMovedEvent& event) -> bool;
        auto onMouseButtonPressedEvent(MouseButtonPressedEvent& event) -> bool;
        auto onMouseButtonReleasedEvent(MouseButtonReleasedEvent& event) -> bool;
        auto onMouseScrollEvent(MouseScrollEvent& event) -> bool;
        auto onKeyPressedEvent(KeyPressedEvent& event) -> bool;
        auto onKeyReleasedEvent(KeyReleasedEvent& event) -> bool;
        auto onKeyTypedEvent(KeyCharEvent& event) -> bool;
        auto onWindowResizedEvent(WindowResizedEvent& event) -> bool;

        /*
         * Helper functions
         * */
        auto updateModifierState() const -> void;
        auto ImGuiToKtKey(Int32_T key) const -> ImGuiKey;
    };

}   // END NAMESPACE kT


#endif //KATE_ENGINE_IMGUI_LAYER_HH
