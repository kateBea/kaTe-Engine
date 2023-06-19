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
#include <Tools/Common.hh>

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

        virtual auto OnAttach() -> void = 0;
        virtual auto OnDetach() -> void = 0;
        virtual auto OnUpdate() -> void = 0;
        virtual auto OnEvent(Event& event) -> void {}
        virtual auto OnImGuiRender() -> void {}

        /**
         * For debugging purposes
         * */
        KT_NODISCARD
        auto GetName() const -> const std::string& { return m_Name; }

    private:
        std::string m_Name{};
    };

}   // END NAMESPACE kaTe


#endif // KATE_ENGINE_LAYER_HH
