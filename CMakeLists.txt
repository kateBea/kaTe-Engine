cmake_minimum_required(VERSION 3.21)
project(kaTe_Engine)

# CXX Lang Requirements
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Libraries
set(GLEW_CMAKE_DIR lib/glew/build/cmake)
set(GLFW_CMAKE_DIR lib/glfw)
set(ENTT_CMAKE_DIR lib/entt)
set(FMT_CMAKE_DIR lib/fmt)

# Projects source files
set(CORE_SOURCES
        src/Logger.cc
        src/LayerStack.cc
        src/ImGuiLayer.cc
        src/OpenGLContext.cc
        src/InputManager.cc src/EditorLayer.cc src/Timer.cc)

set(PLATFORM_SOURCES
        src/Application.cc
        src/MainWindow.cc
        )

set(RENDERER_SOURCES
        src/OpenGLVertexArray.cc
        src/OpenGLVertexBuffer.cc
        src/OpenGLIndexBuffer.cc
        src/Renderer.cc
        src/VertexBuffer.cc
        src/IndexBuffer.cc
        src/OpenGLShader.cc
        src/OpenGLRenderer.cc
        src/RendererAPI.cc
        src/OrthographicCamera.cc src/Texture2D.cc include/Renderer/Material/Texture2D.hh src/OpenGLTexture2D.cc include/Renderer/OpenGL/OpenGLTexture2D.hh src/RenderCommand.cc src/Renderer2D.cc src/OrthographicCameraController.cc src/BaseShader.cc src/FrameBuffer.cc src/OpenGLFrameBuffer.cc src/Camera.cc)

set(IMGUI_SOURCES
        lib/imgui/imgui.cpp
        lib/imgui/backends/imgui_impl_glfw.cpp
        lib/imgui/imgui_widgets.cpp
        lib/imgui/imgui_draw.cpp
        lib/imgui/imgui_tables.cpp
        lib/imgui/imgui_demo.cpp
        lib/imgui/backends/imgui_impl_opengl3.cpp)

set(EDITOR_SOURCES
        src/Editor.cc src/HierarchyPanel.cc src/InspectorPanel.cc src/SettingsPanel.cc src/ScenePanel.cc src/StatsPanel.cc)

set(SCENE_SOURCES
        src/Scene.cc src/SceneCamera.cc)

set(SOURCES
        ${CORE_SOURCES}
        ${PLATFORM_SOURCES}
        ${IMGUI_SOURCES}
        ${RENDERER_SOURCES}
        ${EDITOR_SOURCES}
        ${SCENE_SOURCES}
        src/Engine.cc src/stb_image.cc)

# External dependencies
add_subdirectory(${GLEW_CMAKE_DIR})
add_subdirectory(${GLFW_CMAKE_DIR})
add_subdirectory(${FMT_CMAKE_DIR})
add_subdirectory(${ENTT_CMAKE_DIR})

# Libraries to link against target
set(LIBRARIES glfw glew_s fmt EnTT::EnTT)

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
include_directories(lib/stb/)
include_directories(lib/entt/single_include)

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
target_compile_definitions(kaTe_Engine PRIVATE USE_GLFW_INPUT)
