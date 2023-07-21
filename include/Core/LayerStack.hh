/**
 * LayerStack.hh
 * Created by kate on 5/26/23.
 * */

#ifndef KATE_ENGINE_LAYER_STACK_HH
#define KATE_ENGINE_LAYER_STACK_HH

// C++ Standard Library
#include <memory>

// Project Headers
#include "Layer.hh"
#include "Tools/Common.hh"

namespace kaTe {
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
        explicit LayerStack() = default;

        auto Init() -> void;
        auto shutDown() -> void { /*Empty for now */ }

        auto addLayer(const std::shared_ptr<Layer>& layer) -> void;
        auto addOverlay(const std::shared_ptr<Layer>& overlay) -> void;

        auto popLayer(const std::shared_ptr<Layer>& layer) -> void;
        auto popOverlay(const std::shared_ptr<Layer>& overlay) -> void;

        auto begin() -> std::vector<std::shared_ptr<Layer>>::iterator { return m_Layers.begin(); }
        auto end() -> std::vector<std::shared_ptr<Layer>>::iterator { return m_Layers.end(); }

        auto rbegin() -> std::vector<std::shared_ptr<Layer>>::reverse_iterator { return m_Layers.rbegin(); }
        auto rend() -> std::vector<std::shared_ptr<Layer>>::reverse_iterator { return m_Layers.rend(); }

    private:
        std::vector<std::shared_ptr<Layer>> m_Layers{};
        UInt32_T m_LayerIndex{};
    };

} // END NAMESPACE kT

#endif // KATE_ENGINE_LAYER_STACK_HH
