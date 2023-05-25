cmake_minimum_required(VERSION 3.25)
project(kaTe_Engine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CORE_SOURCES src/logger.cc)

set(SOURCES ${CORE_SOURCES})

include_directories(
        include/
        lib/spdlog/include
)

add_executable(kaTe_Engine src/main.cc ${SOURCES})
