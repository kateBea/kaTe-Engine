/**
 * Logger.hh
 * Created by kate on 5/25/23.
 * */

#ifndef KATE_ENGINE_LOGGER_HH
#define KATE_ENGINE_LOGGER_HH

// C++ Standard Library
#include <memory>

// Third-Party Libraries
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

// Spdlog already has fmt bundled, but we are using it
// as an external library
#include <fmt/format.h>

// Project Headers
#include <Tools/Singleton.hh>
#include <Tools/Common.hh>

namespace kaTe {
    /**
     * Holds the CORE logger and the APP logger. The CORE logger
     * logs information about the current state of the Core systems of the engine.
     * The APP logger logs information about the current state of the Application,
     * which manages and serves as a central Hub for our engine
     * */
    class Logger : public Singleton<Logger> {
    public:
        Logger() : m_CoreLogger{ nullptr }, m_AppLogger{ nullptr }  { init(); }

        auto getCoreLogger() -> const std::shared_ptr<spdlog::logger>&;
        auto getAppLogger() -> const std::shared_ptr<spdlog::logger>&;

    private:
        auto init() -> void;

        std::shared_ptr<spdlog::logger> m_CoreLogger{};
        std::shared_ptr<spdlog::logger> m_AppLogger{};
    };
}

#if defined(NDEBUG) || defined(_DEBUG)
    #define KT_ENABLE_LOGGING
#else
    #undef KT_ENABLE_LOGGING
#endif

// Log macros
#if defined(KT_ENABLE_LOGGING)

    #define KATE_CORE_LOGGER_ERROR(...) kaTe::Logger::get().getCoreLogger()->error(fmt::format(__VA_ARGS__))
    #define KATE_CORE_LOGGER_WARN(...) kT::Logger::get().getCoreLogger()->warn(fmt::format(__VA_ARGS__))
    #define KATE_CORE_LOGGER_CRITICAL(...) kT::Logger::get().getCoreLogger()->critical(fmt::format(__VA_ARGS__))
    #define KATE_CORE_LOGGER_TRACE(...) kT::Logger::get().getCoreLogger()->trace(fmt::format(__VA_ARGS__))
    #define KATE_CORE_LOGGER_INFO(...) kaTe::Logger::get().getCoreLogger()->info(fmt::format(__VA_ARGS__))
    #define KATE_CORE_LOGGER_DEBUG(...) kaTe::Logger::get().getCoreLogger()->debug(fmt::format(__VA_ARGS__))

    #define KATE_APP_LOGGER_ERROR(...) kaTe::Logger::get().getAppLogger()->error(fmt::format(__VA_ARGS__))
    #define KATE_APP_LOGGER_WARN(...) kT::Logger::get().getAppLogger()->warn(fmt::format(__VA_ARGS__))
    #define KATE_APP_LOGGER_CRITICAL(...) kT::Logger::get().getAppLogger()->critical(fmt::format(__VA_ARGS__))
    #define KATE_APP_LOGGER_TRACE(...) kaTe::Logger::get().getAppLogger()->trace(fmt::format(__VA_ARGS__))
    #define KATE_APP_LOGGER_INFO(...) kaTe::Logger::get().getAppLogger()->info(fmt::format(__VA_ARGS__))
    #define KATE_APP_LOGGER_DEBUG(...) kaTe::Logger::get().getAppLogger()->debug(fmt::format(__VA_ARGS__))
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
