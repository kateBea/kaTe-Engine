#include <core/assert.hh>
#include <core/logger.hh>

int main(int, char**) {
    kT::Logger::init();

    KATE_LOGGER_INFO("Info sLogger");
    KATE_LOGGER_CRITICAL("Info sLogger");
    KATE_LOGGER_DEBUG("Info sLogger");
    KATE_LOGGER_ERROR("Info sLogger");
    KATE_LOGGER_WARN("Warn sLogger");
    KATE_LOGGER_TRACE("Trace sLogger");

    return 0;
}