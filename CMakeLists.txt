cmake_minimum_required(VERSION 3.25)
project(kaTe_Engine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CORE_SOURCES src/Logger.cc)

set(SOURCES ${CORE_SOURCES} src/Application.cc)

# Compiler flags
if (MSVC)
    # Compile commands for Windows

else()
    # Compile commands for Linux
    add_compile_options(-fsanitize=address -D_DEBUG)
    add_link_options(-fsanitize=address)
endif()

include_directories(
        include/
        lib/spdlog/include
)

add_executable(kaTe_Engine src/EntryPoint.cc ${SOURCES})
