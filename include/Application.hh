//
// Created by kate on 5/25/23.
//

#ifndef KATE_ENGINE_APPLICATION_HH
#define KATE_ENGINE_APPLICATION_HH

#include <memory>

#if defined(__WIN32__)
    #include "Platform/Window/Win32Window.hh"
#else
    #include "Platform/Window/LinuxWindow.hh"
#endif

namespace kT {
    class Application {
    public:
        Application();

        Application(Application&& other)
            :   m_State{ other.m_State }, m_Window{ std::move(other.m_Window) }
        {}

        ~Application() = default;

        auto init() -> void;
        auto loop() -> void;
        auto shutdown() -> void;
    private:
        enum class State {
            NONE,
            RUNNING,
            STOPPED,
            COUNT,
        };

        // Helper functions

        // Member variables
        State m_State{};
        std::unique_ptr<Window> m_Window{};

    };
}


#endif //KATE_ENGINE_APPLICATION_HH
