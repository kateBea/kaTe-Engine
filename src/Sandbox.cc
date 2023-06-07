/**
 * Sandbox.cc
 * Created by kate on 6/7/23.
 * */

#include <Sandbox.hh>

#include <Core/Logger.hh>
#include <Core/EngineManager.hh>

#include <Core/Layers/ImGuiLayer.hh>
#include <Core/Layers/TestLayer.hh>


namespace kaTe {
    auto Sandbox::run() -> kaTe::Int32_T {
        EngineManager& engine{ EngineManager::get() };

        try {
            engine.init();
            engine.pushLayer(std::make_shared<TestLayer>());
            engine.pushOverlay(std::make_shared<ImGuiLayer>());

            while (engine.isRunning()) {
                engine.updateState();
            }
        }
        catch(const std::exception& exception) {
            KATE_APP_LOGGER_CRITICAL("Engine exception thrown.\n Message: {}", exception.what());
            return 1;
        }
        catch(...) {
            KATE_APP_LOGGER_CRITICAL("Engine exception thrown.\n Unknown type of exception");
            return 1;
        }

        engine.shutDown();
        return 0;
    }
}