//
// Created by kate on 6/23/23.
//

#ifndef KATE_ENGINE_PANEL_HH
#define KATE_ENGINE_PANEL_HH

#include <Tools/Common.hh>
#include <Core/Events/Event.hh>

namespace kaTe {
    /**
     * General interface for ImGui Panels
     * */
    class Panel {
    public:
        // TODO: take path to icon for the panel
        explicit Panel() = default;
        virtual ~Panel() = default;

        Panel(const Panel& other) = default;
        Panel(Panel&& other) = default;

        auto operator=(const Panel& other) -> Panel& = default;
        auto operator=(Panel&& other) -> Panel& = default;

        virtual auto OnUpdate() -> void = 0;
        virtual auto OnEvent(Event& event) -> void = 0;

        virtual auto MakeVisible(bool value) -> void = 0;

        KT_NODISCARD virtual auto IsHovered() const -> bool = 0;
        KT_NODISCARD virtual auto IsFocused() const -> bool = 0;
        KT_NODISCARD virtual auto IsVisible() const -> bool = 0;
    };
}

#endif//KATE_ENGINE_PANEL_HH
