/**
 * ImGuiLayer.cc
 * Created by kate on 5/28/23.
 * */

// C++ Standard Library
#include <any>

// Third-Party Libraries
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

// Project Headers
#include <Core/Logger.hh>
#include <Core/KeyCodes.hh>

#include "Core/EngineManager.hh"
#include <Tools/Common.hh>

#include <Core/Layers/ImGuiLayer.hh>
#include "Platform/InputManager.hh"

namespace kaTe {
    ImGuiLayer::ImGuiLayer() noexcept
        :   Layer{ "ImGuiLayer" } {}

    auto ImGuiLayer::onAttach() -> void {
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

        // When viewports are enabled, we tweak WindowRounding/WindowBg so platform windows can
        // look identical to regular ones.
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        // Configure ImGui Style
        ImGui::StyleColorsDark();
        setupCustomImGuiStyle();
        io.Fonts->AddFontFromFileTTF("../assets/Fonts/Inter/Inter-VariableFont.ttf", 15);

        try {
            // We expect the native window for Linux Window to be a GLFWwindow*
            GLFWwindow* window{ std::any_cast<GLFWwindow*>(EngineManager::get().getMainWindow().getNativeWindow()) };

            // Setup Platform/Renderer backends
            ImGui_ImplGlfw_InitForOpenGL(window, true);
            ImGui_ImplOpenGL3_Init("#version 410");
        }
        catch (const std::bad_any_cast& exception) {
            KATE_APP_LOGGER_ERROR("Exception thrown std::any_cast. What: {}", exception.what());
        }
    }

    auto ImGuiLayer::onDetach() -> void {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    auto ImGuiLayer::onUpdate() -> void {

    }

    auto ImGuiLayer::setupCustomImGuiStyle() -> void {
        // Setup Dear ImGui style
        ImGuiStyle &style = ImGui::GetStyle();

        style.Colors[ImGuiCol_TitleBg] = ImVec4(0.16f, 0.16f, 0.16f, 1.0f);
        style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
        style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);

        style.Colors[ImGuiCol_Tab] = ImVec4(0.16f, 0.16f, 0.16f, 1.0f);

        style.Colors[ImGuiCol_Button] = ImVec4(0.16f, 0.16f, 0.16f, 1.0f);
        style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.26f, 0.26f, 1.0f);
        style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);

        style.Colors[ImGuiCol_TabHovered] = ImVec4(0.26f, 0.26f, 0.26f, 1.0f);
        style.Colors[ImGuiCol_TabActive] = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
        style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.16f, 0.16f, 0.16f, 1.0f);
        style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.0f);

        style.Colors[ImGuiCol_Header] = ImVec4(0.16f, 0.16f, 0.16f, 1.0f);
        style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.26f, 0.26f, 1.0f);
        style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);

        style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.16f, 0.16f, 0.16f, 1.0f);

        style.Colors[ImGuiCol_FrameBg] = ImVec4(0.16f, 0.16f, 0.16f, 1.0f);
        style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
        style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);

        style.Colors[ImGuiCol_Border] = ImVec4(0.26f, 0.26f, 0.26f, 1.0f);
        style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.16f, 0.16f, 0.16f, 1.0f);

        style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.10f, 0.10f, 0.10f, 1.0f);
        style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
        style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.16f, 0.16f, 0.16f, 1.0f);


        // borders
        style.WindowBorderSize = 0.0f;
        style.FrameBorderSize = 0.0f;
        style.PopupBorderSize = 0.0f;

        // Rounding values
        style.FrameRounding = 5.0f;
        style.GrabRounding = 5.0f;
        style.ChildRounding = 5.0f;
        style.WindowRounding = 5.0f;
        style.PopupRounding = 5.0f;
        style.ScrollbarRounding = 5.0f;
        style.TabRounding = 5.0f;
    }

    auto ImGuiLayer::beginFrame() -> void {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    auto ImGuiLayer::endFrame() -> void {
        ImGuiIO& io{ ImGui::GetIO() };
        Window& win{ EngineManager::get().getMainWindow() };
        io.DisplaySize = ImVec2(win.getWidth(), win.getHeight());

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Update and Render additional Platform Windows
        // (Platform functions may change the current OpenGL context,
        // so we save/restore it to make it easier to paste this code elsewhere.
        //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            GLFWwindow* backupCurrentContext{ glfwGetCurrentContext() };
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backupCurrentContext);
        }
    }

    auto ImGuiLayer::onImGuiRender() -> void {
        bool bl{ true };
        ImGui::ShowDemoWindow(&bl);
        ImGui::Begin("Info");
        ImGui::SetWindowSize("Test", { 128, 64 });
        ImGui::Text("Framerate: %.1f", ImGui::GetIO().Framerate);
        ImGui::End();
    }

    ImGuiLayer::~ImGuiLayer() = default;
}
