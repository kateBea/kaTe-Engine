/**
 * Application.hh
 * Created by kate on 5/25/23.
 * */

#ifndef KATE_ENGINE_APPLICATION_HH
#define KATE_ENGINE_APPLICATION_HH

// C++ Standard Library
#include <memory>

// Project Headers
#if defined(_WIN64) || defined(_WIN32)
    #include <Platform/Window/WinWindow.hh>
#else
    #include <Platform/Window/kateGLFWwindow.hh>
#endif

#include <Tools/Singleton.hh>

#include <Core/Assert.hh>
#include <Core/Events/AppEvents.hh>
#include <Core/Events/Event.hh>
#include <Core/Layers/LayerStack.hh>
#include <Platform/Window/InputManager.hh>


namespace kaTe {
    /**
     * This class is essentially a wrapper around all the subsystems of our
     * application, and it serves as a way of communicating the different
     * components of ours engine
     * */
    class Application : public Singleton<Application> {
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
        auto getWindow() -> Window&;

        KT_NODISCARD
        auto getInputManager() -> InputManager&;

        /**
         * Initializes the subsystems of the
         * application
         * */
        auto init() -> void;

        /**
         * Executes the main loop which keeps
         * our application running
         * */
        auto loop() -> void;

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



        /*************************************************************
         *  HELPER FUNCTIONS -----------------------------------------
         * ********************************************************+ */

        /**
         * Initializes the main Window
         * */
        auto initWindow() -> void;

        /**
         * Initializes the Layer Stack
         * */
        auto initLayerStack() -> void;

        auto initInputManager() -> void;

        // MEMBER VARIABLES ----------------------------
        State m_State{ State::RUNNING };

        Ptr_T<Window> m_Window{ nullptr };
        Ptr_T<LayerStack> m_LayerStack{ nullptr };

        // For now, there's one instance of InputManager in our application
        // In case we may want to poll input from multiple Windows, this
        // could become part of the Window itself as an aggregation
        Ptr_T<InputManager> m_InputManager{ nullptr };
    };
}


#endif //KATE_ENGINE_APPLICATION_HH
