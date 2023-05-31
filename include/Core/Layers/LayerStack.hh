/**
 * LayerStack.hh
 * Created by kate on 5/26/23.
 * */

#ifndef KATE_ENGINE_LAYER_STACK_HH
#define KATE_ENGINE_LAYER_STACK_HH

// C++ Standard Library
#include <memory>

// Project Headers
#include <Core/Layers/Layer.hh>

namespace kT {
    /**
     * Defines the order of handling of the attached layers.
     * This helps task to be done in their appropriate order.
     * It is not implemented as and std::stack as we would want to
     * insert elements not at one end, we could maybe use a std::priority_queue
     * and implement a custom sorter, but to simplify things, and std::vector is used
     * for now even tho is not very performant for insertions
     *
     * Layer stack is owned by the application but Layers are supposed to live
     * throughout the lifetime of our main application meaning if we pop a layer
     * it simply becomes independent to the Layer stack. Layers lifetime does not
     * need to be tied to the layer stack
     * */
    class LayerStack {
    public:
        using LayerPtr = std::shared_ptr<Layer>;
        using LayerContainerIter = std::vector<LayerPtr>::iterator;
        using LayerContainerReverseIter = std::vector<LayerPtr>::reverse_iterator;

        explicit LayerStack() : m_Layers{}, m_LayerIndex{ m_Layers.begin() } {}

        auto init() -> void { /*Empty for now */ }
        auto shutDown() -> void { /*Empty for now */ }

        auto addLayer(LayerPtr layer) -> void;
        auto addOverlay(LayerPtr overlay) -> void;

        auto popLayer(LayerPtr layer) -> void;
        auto popOverlay(LayerPtr overlay) -> void;

        auto begin() -> LayerContainerIter { return m_Layers.begin(); }
        auto end() -> LayerContainerIter { return m_Layers.end(); }

        auto rbegin() -> LayerContainerReverseIter { return m_Layers.rbegin(); }
        auto rend() -> LayerContainerReverseIter { return m_Layers.rend(); }

    private:
        std::vector<LayerPtr> m_Layers{};
        LayerContainerIter m_LayerIndex{};
    };

} // END NAMESPACE kT

#endif // KATE_ENGINE_LAYER_STACK_HH
