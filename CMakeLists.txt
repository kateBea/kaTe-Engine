cmake_minimum_required(VERSION 3.21)
project(kaTe_Engine)

# CXX Lang Requirements
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Libraries
set(GLEW_CMAKE_DIR lib/glew/build/cmake)
set(GLFW_CMAKE_DIR lib/glfw)
set(FMT_CMAKE_DIR lib/fmt)

# Projects source files
set(CORE_SOURCES
        src/Logger.cc
        src/LayerStack.cc
        src/ImGuiLayer.cc
        src/OpenGLContext.cc)

set(PLATFORM_SOURCES
        src/Application.cc
        src/kaTeGLFWwindow.cc
        src/WinWindow.cc
        src/LinuxInputManager.cc)

set(RENDERER_SOURCES
        src/OpenGLVertexArray.cc
        src/OpenGLVertexBuffer.cc
        src/OpenGLIndexBuffer.cc
        src/Renderer.cc
        src/VertexBuffer.cc
        src/IndexBuffer.cc
        src/Shader.cc
        src/OpenGLRenderer.cc
        src/RendererAPI.cc
        src/OrthographicCamera.cc)

set(IMGUI_SOURCES
        lib/imgui/imgui.cpp
        lib/imgui/backends/imgui_impl_glfw.cpp
        lib/imgui/imgui_widgets.cpp
        lib/imgui/imgui_draw.cpp
        lib/imgui/imgui_tables.cpp
        lib/imgui/imgui_demo.cpp
        lib/imgui/backends/imgui_impl_opengl3.cpp)

set(SOURCES
        ${CORE_SOURCES}
        ${PLATFORM_SOURCES}
        ${IMGUI_SOURCES}
        ${RENDERER_SOURCES})

# External dependencies
add_subdirectory(${GLEW_CMAKE_DIR})
add_subdirectory(${GLFW_CMAKE_DIR})
add_subdirectory(${FMT_CMAKE_DIR})

# Libraries to link against target
set(LIBRARIES glfw glew_s fmt)

# Compiler flags
if (MSVC)
    # Compile commands for Windows

else()
    # Compile commands for Linux
    add_compile_options(-fsanitize=address -D_DEBUG -Wall -Wextra -Wpedantic)
    # Sanitize
    add_link_options(-fsanitize=address)
endif()

# Header files directories
include_directories(include/)
include_directories(lib/spdlog/include)
include_directories(lib/glfw/include)
include_directories(lib/glew/include)
include_directories(lib/imgui)
include_directories(lib/imgui/backends)
include_directories(lib/glm/)

add_executable(kaTe_Engine src/EntryPoint.cc ${SOURCES})
target_link_libraries(kaTe_Engine ${LIBRARIES})

# Avoid GLFW including OpenGL headers
target_compile_definitions(kaTe_Engine PRIVATE GLFW_INCLUDE_NONE)

# Needed if we want to use FMT as an external library and not the
# one bundled with SPDLOG
# For more: https://github.com/gabime/spdlog/wiki/0.-FAQ
target_compile_definitions(kaTe_Engine PRIVATE SPDLOG_FMT_EXTERNAL)

# For Key and Mouse Codes we use the GLFW ones
target_compile_definitions(kaTe_Engine PRIVATE USE_GLFW_KEY_CODES)
target_compile_definitions(kaTe_Engine PRIVATE USE_GLFW_MOUSE_CODES)
