//
// Created by kate on 5/26/23.
//

#ifndef KATE_ENGINE_LAYER_HH
#define KATE_ENGINE_LAYER_HH

#include <string_view>
#include <string>

#include "Core/Events/Event.hh"

namespace kT {

    /**
     * Describes a modular entity that helps encapsulate and organize
     * functionality of the engine, i.e. subsystems of our engine, e.g.:
     * Render Layer which would handle rendering pipeline and graphics, amongst others.
     *
     * This class serves simply as a general interface for Layers
     * */
    class Layer {
    public:
        explicit Layer(std::string_view name = "Base Layer")
            :   m_Name{ name } {}

        virtual ~Layer() = default;

        virtual auto onAttach() -> void = 0;
        virtual auto onDetach() -> void = 0;
        virtual auto onUpdate() -> void = 0;
        virtual auto onEvent(Event& event) -> void = 0;

        /**
         * For debugging purposes
         * */
        [[nodiscard]]
        auto getName() const -> const std::string& { return m_Name; }

    private:

        std::string m_Name{};
    };
}


#endif // KATE_ENGINE_LAYER_HH
