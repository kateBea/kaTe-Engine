//
// Created by kate on 5/27/23.
//

#ifndef KATE_ENGINE_EXAMPLELAYER_HH
#define KATE_ENGINE_EXAMPLELAYER_HH

#include "Layer.hh"
#include "../Logger.hh"

namespace kT {
    class ExampleLayer : public Layer {
    public:
        ExampleLayer()
                : Layer{ "Example" } {}

        auto onAttach() -> void override {}
        auto onDetach() -> void override {}
        auto onUpdate() -> void override { KATE_APP_LOGGER_INFO("ExampleLayer::OnUpdate()"); }
        auto onEvent(Event& event) -> void override { KATE_APP_LOGGER_TRACE("ExampleLayer::onEvent({})", event.displayData()); }
    private:

    };
}
#endif //KATE_ENGINE_EXAMPLELAYER_HH
