#include "../include/Core/Logger.hh"
#include "../include/Core/Assert.hh"

namespace kT {
    // Initialize sLogger
    std::shared_ptr<spdlog::logger> Logger::sLogger{};

    // Don't forget to call this function before using the sLogger,
    // or you will get null pointer read exceptions
    auto Logger::init() -> void {
        sLogger = spdlog::stdout_color_mt("KATE_CORE_LOGGER");

        // Set sLogger pattern. Check out spdlog wiki for info about formatting
        // https://github.com/gabime/spdlog/wiki/3.-Custom-formatting
        sLogger->set_pattern("%^[%X] [thread %t] %v%$");
        sLogger->set_level(spdlog::level::trace);
    }

    auto Logger::getCoreLogger() -> const std::shared_ptr<spdlog::logger>& {
        KT_ASSERT(sLogger && "sLogger is NULL. Forgot to call kT::Logger::init()?");

        return sLogger;
    }
}