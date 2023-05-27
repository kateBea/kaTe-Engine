//
// Created by kate on 5/25/23.
//

#ifndef KATE_ENGINE_LOGGER_HH
#define KATE_ENGINE_LOGGER_HH

#include <memory>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <fmt/format.h>

namespace kT {
    class Logger {
    public:
        /**
         * Initializes the Logger. Don't forget to call this function before
         * using any of the available methods
         * */
        static auto init() -> void;

        static auto getCoreLogger() -> const std::shared_ptr<spdlog::logger>&;
        static auto getAppLogger() -> const std::shared_ptr<spdlog::logger>&;


    private:
        static std::shared_ptr<spdlog::logger> sCoreLogger;
        static std::shared_ptr<spdlog::logger> sAppLogger;
    };
}

#if defined(NDEBUG) || defined(_DEBUG)
    #define KT_ENABLE_LOGGING
#else
    #undef KT_ENABLE_LOGGING
#endif

// Log macros
#if defined(KT_ENABLE_LOGGING)

    #define KATE_CORE_LOGGER_ERROR(...) kT::Logger::getCoreLogger()->error(fmt::format(__VA_ARGS__))
    #define KATE_CORE_LOGGER_WARN(...) kT::Logger::getCoreLogger()->warn(fmt::format(__VA_ARGS__))
    #define KATE_CORE_LOGGER_CRITICAL(...) kT::Logger::getCoreLogger()->critical(fmt::format(__VA_ARGS__))
    #define KATE_CORE_LOGGER_TRACE(...) kT::Logger::getCoreLogger()->trace(fmt::format(__VA_ARGS__))
    #define KATE_CORE_LOGGER_INFO(...) kT::Logger::getCoreLogger()->info(fmt::format(__VA_ARGS__))
    #define KATE_CORE_LOGGER_DEBUG(...) kT::Logger::getCoreLogger()->debug(fmt::format(__VA_ARGS__))

    #define KATE_APP_LOGGER_ERROR(...) kT::Logger::getAppLogger()->error(fmt::format(__VA_ARGS__))
    #define KATE_APP_LOGGER_WARN(...) kT::Logger::getAppLogger()->warn(fmt::format(__VA_ARGS__))
    #define KATE_APP_LOGGER_CRITICAL(...) kT::Logger::getAppLogger()->critical(fmt::format(__VA_ARGS__))
    #define KATE_APP_LOGGER_TRACE(...) kT::Logger::getAppLogger()->trace(fmt::format(__VA_ARGS__))
    #define KATE_APP_LOGGER_INFO(...) kT::Logger::getAppLogger()->info(fmt::format(__VA_ARGS__))
    #define KATE_APP_LOGGER_DEBUG(...) kT::Logger::getAppLogger()->debug(fmt::format(__VA_ARGS__))
#else
    #define KATE_CORE_LOGGER_ERROR(...)
    #define KATE_CORE_LOGGER_WARN(...)
    #define KATE_CORE_LOGGER_CRITICAL(...)
    #define KATE_CORE_LOGGER_TRACE(...)
    #define KATE_CORE_LOGGER_INFO(...)
    #define KATE_CORE_LOGGER_DEBUG(...)

    #define KATE_APP_LOGGER_ERROR(...)
    #define KATE_APP_LOGGER_WARN(...)
    #define KATE_APP_LOGGER_CRITICAL(...)
    #define KATE_APP_LOGGER_TRACE(...)
    #define KATE_APP_LOGGER_INFO(...)
    #define KATE_APP_LOGGER_DEBUG(...)
#endif


#endif // LOGGER_H
