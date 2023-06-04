/**
 * ExampleLayer.hh
 * Created by kate on 5/27/23.
 * */

#ifndef KATE_ENGINE_EXAMPLE_LAYER_HH
#define KATE_ENGINE_EXAMPLE_LAYER_HH

#include <Core/Layers/Layer.hh>
#include <Core/Logger.hh>

namespace kaTe {
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

}   // END NAMESPACE kT

#endif // KATE_ENGINE_EXAMPLE_LAYER_HH
