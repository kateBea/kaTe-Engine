//
// Created by kate on 6/23/23.
//

#ifndef KATE_ENGINE_PANEL_HH
#define KATE_ENGINE_PANEL_HH

namespace kaTe {
    /**
     * General interface for ImGui Panels
     * */
    class Panel {
        virtual auto OnAttach() -> void = 0;
        virtual auto OnUpdate() -> void = 0;
        virtual auto OnEvent() -> void = 0;
        virtual auto OnImGuiRender() -> void = 0;

        virtual auto IsHovered() -> bool = 0;
        virtual auto IsFocused() -> bool = 0;
        virtual auto IsVisible() -> void = 0;
    };
}

#endif//KATE_ENGINE_PANEL_HH
