cmake_minimum_required(VERSION 3.25)
project(kaTe_Engine)

# CXX Lang Requirements
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Libraries
set(GLEW_CMAKE_DIR lib/glew/build/cmake)
set(GLFW_CMAKE_DIR lib/glfw)
set(FMT_CMAKE_DIR lib/fmt)

# Projects source files
set(CORE_SOURCES src/Logger.cc src/LayerStack.cc)
set(PLATFORM_SOURCES src/Application.cc src/LinuxWindow.cc src/Win32Window.cc)

set(SOURCES ${CORE_SOURCES} ${PLATFORM_SOURCES})

# External dependencies
add_subdirectory(${GLEW_CMAKE_DIR})
add_subdirectory(${GLFW_CMAKE_DIR})
add_subdirectory(${FMT_CMAKE_DIR})

# Libraries to link against target
set(LIBRARIES glfw glew_s fmt::fmt-header-only)

# Compiler flags
if (MSVC)
    # Compile commands for Windows

else()
    # Compile commands for Linux
    add_compile_options(-fsanitize=address -D_DEBUG)
    add_link_options(-fsanitize=address)
endif()

# Header files directories
include_directories(include/)
include_directories(lib/spdlog/include)
include_directories(lib/glfw/include)
include_directories(lib/glew/include)

add_executable(kaTe_Engine src/EntryPoint.cc ${SOURCES})
target_link_libraries(kaTe_Engine ${LIBRARIES})

# Avoid GLFW including OpenGL headers
target_compile_definitions(kaTe_Engine PRIVATE GLFW_INCLUDE_NONE)

# Needed if we want to use FMT as an external library and not the
# one bundled with SPDLOG
# For more: https://github.com/gabime/spdlog/wiki/0.-FAQ
target_compile_definitions(kaTe_Engine PRIVATE SPDLOG_FMT_EXTERNAL)
