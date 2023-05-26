#include "../include/Core/Logger.hh"
#include "../include/Core/Assert.hh"

namespace kT {
    // Initialize loggers
    std::shared_ptr<spdlog::logger> Logger::sCoreLogger{};
    std::shared_ptr<spdlog::logger> Logger::sAppLogger{};

    // Don't forget to call this function before using the loggers
    auto Logger::init() -> void {
        sCoreLogger = spdlog::stdout_color_mt("KATE_CORE_LOGGER");
        sAppLogger = spdlog::stdout_color_mt("KATE_APP_LOGGER");

        // Set sLogger pattern.
        // Check out the wiki for info about formatting
        // https://github.com/gabime/spdlog/wiki/3.-Custom-formatting
        sCoreLogger->set_pattern("%^[%X] CORE LOG [thread %t] %v%$");
        sCoreLogger->set_level(spdlog::level::trace);

        // Set sLogger pattern.
        // Check out the wiki for info about formatting
        // https://github.com/gabime/spdlog/wiki/3.-Custom-formatting
        sAppLogger->set_pattern("%^[%X] APP LOG [thread %t] %v%$");
        sAppLogger->set_level(spdlog::level::trace);
    }

    auto Logger::getCoreLogger() -> const std::shared_ptr<spdlog::logger>& {
        KT_ASSERT(sCoreLogger, "CORE LOGGER is NULL. Forgot to call kT::Logger::init()?");

        return sCoreLogger;
    }

    auto Logger::getAppLogger() -> const std::shared_ptr<spdlog::logger>& {
        KT_ASSERT(sAppLogger, "APP LOGGER is NULL. Forgot to call kT::Logger::init()?");

        return sAppLogger;
    }
}