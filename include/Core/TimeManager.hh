//
// Created by kate on 6/8/23.
//

#ifndef KATE_ENGINE_TIME_STEP_HH
#define KATE_ENGINE_TIME_STEP_HH

#include <ratio>
#include <chrono>

namespace kaTe {
    enum class TimeUnit {
        NONE,
        SECONDS,
        MILLISECONDS,
        MICROSECONDS,
        NANOSECONDS,
        COUNT,
    };

    class TimeManager {
    public:
        auto updateDeltaTime() -> void {
            TimePoint_T now{ Clock_T::now() };
            m_TimeStep = std::chrono::duration_cast<Sec_T>(now - m_LasFrameTime).count();
            m_LasFrameTime = now;
        }

        auto getDeltaTime(TimeUnit unit = TimeUnit::SECONDS) -> double {
            switch (unit) {
                case TimeUnit::SECONDS:         return m_TimeStep;
                case TimeUnit::MILLISECONDS:    return m_TimeStep / SEC_TO_MILLI;
                case TimeUnit::MICROSECONDS:    return m_TimeStep / SEC_TO_MICRO;
                case TimeUnit::NANOSECONDS:     return m_TimeStep / SEC_TO_NANO;
            }
        }

        // Returns amount of time between this time_point and the clock's epoch
        static auto getTime(TimeUnit unit = TimeUnit::SECONDS) -> double {
            switch (unit) {
                case TimeUnit::SECONDS:         return Clock_T::now().time_since_epoch().count();
                case TimeUnit::MILLISECONDS:    return Clock_T::now().time_since_epoch().count() / SEC_TO_MILLI;
                case TimeUnit::MICROSECONDS:    return Clock_T::now().time_since_epoch().count() / SEC_TO_MICRO;
                case TimeUnit::NANOSECONDS:     return Clock_T::now().time_since_epoch().count() / SEC_TO_NANO;
            }
        }

    private:
        static constexpr double SEC_TO_MILLI{ 1000.0 };
        static constexpr double SEC_TO_MICRO{ 1000'000.0 };
        static constexpr double SEC_TO_NANO{ 1000'000'000.0 };

        using Nano_T = std::chrono::duration<double, std::ratio<1, 1000000000>>;
        using Micro_T = std::chrono::duration<double, std::ratio<1, 1000000>>;
        using Milli_T = std::chrono::duration<double, std::ratio<1, 1000>>;
        using Sec_T = std::chrono::duration<double, std::ratio<1, 1>>;
        using Clock_T = std::chrono::high_resolution_clock;
        using TimePoint_T   =  std::chrono::time_point<Clock_T>;

        // Time in seconds
        double m_TimeStep{};

        TimePoint_T m_LasFrameTime{};
    };
}


#endif//KATE_ENGINE_TIME_STEP_HH
