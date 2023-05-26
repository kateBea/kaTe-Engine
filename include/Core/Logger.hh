//
// Created by kate on 5/25/23.
//

#ifndef KATE_ENGINE_LOGGER_HH
#define KATE_ENGINE_LOGGER_HH

#include <memory>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace kT {
    class Logger {
    public:
        /**
         * Initializes the Logger. Don't forget to call this function before
         * using any of the available methods
         * */
        static auto init() -> void;

        static auto getCoreLogger() -> const std::shared_ptr<spdlog::logger>&;


    private:
        static std::shared_ptr<spdlog::logger> sLogger;
    };
}

// Log macros

#if defined(KT_ENABLE_LOGGING)
    #define KATE_LOGGER_ERROR(...) kT::Logger::getCoreLogger()->error(__VA_ARGS__)
    #define KATE_LOGGER_WARN(...) kT::Logger::getCoreLogger()->warn(__VA_ARGS__)
    #define KATE_LOGGER_CRITICAL(...) kT::Logger::getCoreLogger()->critical(__VA_ARGS__)
    #define KATE_LOGGER_TRACE(...) kT::Logger::getCoreLogger()->trace(__VA_ARGS__)
    #define KATE_LOGGER_INFO(...) kT::Logger::getCoreLogger()->info(__VA_ARGS__)
    #define KATE_LOGGER_DEBUG(...) kT::Logger::getCoreLogger()->debug(__VA_ARGS__)
#endif


#endif // LOGGER_H
