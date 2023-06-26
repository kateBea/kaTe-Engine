//
// Created by kate on 6/22/23.
//

#ifndef KATE_ENGINE_EDITOR_HH
#define KATE_ENGINE_EDITOR_HH

#include <string_view>

#include <imgui.h>

namespace kaTe::Editor {

    struct DockControlFlags {
        bool applicationCloseFlag{};
    };

    auto SetupCustomImGuiStyle() -> void;
    auto ShowDockingDisabledMessage() -> void;
    auto HelpMarker(std::string_view description) -> void;
    auto DisplayDockSpace(DockControlFlags& flags) -> void;


}
#endif//KATE_ENGINE_EDITOR_HH
