/**
 * Layer.hh
 * Created by kate on 5/26/23.
 * */

#ifndef KATE_ENGINE_LAYER_HH
#define KATE_ENGINE_LAYER_HH

// C++ Standard Library
#include <string_view>
#include <string>

// Project Headers
#include <Core/Events/Event.hh>

namespace kaTe {

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
        virtual auto onEvent(Event& event) -> void {}

        /**
         * For debugging purposes
         * */
        [[nodiscard]]
        auto getName() const -> const std::string& { return m_Name; }

    private:

        std::string m_Name{};
    };

}   // END NAMESPACE kT


#endif // KATE_ENGINE_LAYER_HH
