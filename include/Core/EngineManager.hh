/**
 * Application.hh
 * Created by kate on 5/25/23.
 * */

#ifndef KATE_ENGINE_ENGINE_MANAGER_HH
#define KATE_ENGINE_ENGINE_MANAGER_HH

// C++ Standard Library
#include <memory>

// Project Headers
#include "Platform/Window/WindowGLFW.hh"

#include "Tools/Singleton.hh"

#include "Assert.hh"
#include "Core/Events/AppEvents.hh"
#include "Core/Events/Event.hh"
#include "Core/Layers/LayerStack.hh"
#include "Platform/InputManager.hh"

// TODO: Temporary just to try shader class
#include "Renderer/IndexBuffer.hh"
#include "Renderer/Renderer.hh"
#include "Renderer/Shader.hh"
#include "Renderer/VertexBuffer.hh"


namespace kaTe {
    /**
     * This class is essentially a wrapper around all the subsystems of our
     * application, and it serves as a way of communicating the different
     * components of ours engine
     * */
    class EngineManager : public Singleton<EngineManager> {
    public:
        /**
         * This functions forwards the received event to the appropriate
         * event handler. For more see the CALLBACK HANDLERS section of this file
         * @param event event to be dispatched
         * */
        auto onEvent(Event& event) -> void;

        /**
         * Adds a the <code>layer</code> to the Layer stack
         * @param layer layer to be stacked
         * */
        auto pushLayer(LayerStack::LayerPtr layer) -> void;

        /**
         * Adds a the <code>overlay</code> to the Layer stack
         * @param overlay overlay to be stacked
         * */
        auto pushOverlay(LayerStack::LayerPtr overlay) -> void;

        /**
         * Returns a reference to the Application main window
         * @returns main window
         * */
        KT_NODISCARD
        auto getMainWindow() -> Window&;

        KT_NODISCARD
        auto getInputManager() -> InputManager&;

        auto getRenderer() ->  std::shared_ptr<Renderer> { KT_ASSERT(m_Renderer, "Renderer is NULL"); return m_Renderer; }

        /**
         * Initializes the subsystems of the
         * application
         * */
        auto init() -> void;

        auto updateState() -> void;

        auto isRunning() -> bool;

        /**
         * Destroys this application freeing
         * any resources it may own
         * */
        auto shutDown() -> void;

    private:
        /*************************************************************
         *  INTERNAL ALIASES -----------------------------------------
         * ********************************************************+ */

        /**
         * Represents the current state of this application
         * */
        enum class State {
            NONE,
            RUNNING,
            STOPPED,
            COUNT,
        };

        /*************************************************************
         * APPLICATION CALLBACK HANDLERS -----------------------------
         * ********************************************************+ */

        /**
         * Callback function for WindowClose event. For now
         * it always returns true as we have no needs to
         * forward this event any further and handler it right away
         * */
        bool onWindowClose(WindowCloseEvent &ev);
        
        /**
         * Callback function for WindowResizedEvent event. For now
         * it always returns true as we have no needs to
         * forward this event any further and handler it right away
         * */
        bool onResizeEvent(WindowResizedEvent &ev);
       

        /*************************************************************
         *  HELPER FUNCTIONS -----------------------------------------
         * ********************************************************+ */

        auto initWindow() -> void;
        auto initLayerStack() -> void;
        auto initInputManager() -> void;
        auto initRenderer() -> void;

        // MEMBER VARIABLES ----------------------------
        State m_State{ State::RUNNING };

        std::unique_ptr<Window> m_Window{ nullptr };
        std::unique_ptr<LayerStack> m_LayerStack{ nullptr };

        // For now, there's one instance of InputManager in our application
        // In case we may want to poll input from multiple Windows, this
        // could become part of the Window itself as an aggregation
        std::unique_ptr<InputManager> m_InputManager{ nullptr };
        std::shared_ptr<Renderer> m_Renderer{ nullptr };

    };
}


#endif//KATE_ENGINE_ENGINE_MANAGER_HH
