/**
 * Sandbox.cc
 * Created by kate on 6/7/23.
 * */

#include <fmt/ranges.h>

#include <Engine.hh>

#include <Core/Application.hh>
#include <Core/Logger.hh>

#include "Core/Timer.hh"
#include <Core/Layers/ImGuiLayer.hh>
#include <Core/Layers/SandBox2D.hh>


namespace kaTe {
    auto Engine::run() -> kaTe::Int32_T {
        Application& application{ Application::Get() };

        try {
            application.Init();
            application.PushLayer(std::make_shared<SandBox2D>());
            application.PushOverlay(std::make_shared<ImGuiLayer>());

            while (application.IsRunning()) {
                application.UpdateState();
            }

            application.ShutDown();
        }
        catch(const std::exception& exception) {
            KATE_APP_LOGGER_CRITICAL("Engine exception thrown.\n Message: {}", exception.what());
            return 1;
        }
        catch(...) {
            KATE_APP_LOGGER_CRITICAL("Engine exception thrown.\n Unknown type of exception");
            return 1;
        }

        return 0;
    }
}