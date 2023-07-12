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
#include <backends/imgui_impl_vulkan.h>

// Project Headers
#include <Tools/Common.hh>

#include <Core/Logger.hh>
#include <Core/KeyCodes.hh>

#include <Core/Application.hh>

#include "Renderer/Renderer.hh"
#include <Core/Layers/ImGuiLayer.hh>
#include <Editor/Editor.hh>
#include <Platform/InputManager.hh>

namespace kaTe {
   ImGuiLayer::ImGuiLayer() noexcept
       :   Layer{ "ImGuiLayer" } {}

   auto ImGuiLayer::OnAttach() -> void {
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
       ImGuiStyle& style{ ImGui::GetStyle() };
       if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
           style.WindowRounding = 0.0f;
           style.Colors[ImGuiCol_WindowBg].w = 1.0f;
       }

       // Configure ImGui Style
       ImGui::StyleColorsDark();
       Editor::ThemeDarkModeDefault();

       // Load fonts
       io.Fonts->AddFontFromFileTTF("../assets/Fonts/Open_Sans/static/OpenSans-Bold.ttf", 17.5);
       io.FontDefault = io.Fonts->AddFontFromFileTTF("../assets/Fonts/Open_Sans/OpenSans-VariableFont_wdth,wght.ttf", 17.5);

       try {
           // We expect the native window for Linux Window to be a GLFWwindow*
           GLFWwindow* window{ std::any_cast<GLFWwindow*>(Application::Get().GetMainWindow().GetNativeWindow()) };

           switch(Renderer::GetActiveGraphicsAPI()) {
               case Renderer::GraphicsAPI::OPENGL_API:
                   m_UseOpenGL = true;
                   break;
               default:
                   m_UseOpenGL = false;
                   break;
           }

           if (m_UseOpenGL) {
               // Setup Platform/Renderer backends
               ImGui_ImplGlfw_InitForOpenGL(window, true);
               const std::string openglVersion{ fmt::format("#version {}{}0", KT_OPENGL_VERSION_MAJOR, KT_OPENGL_VERSION_MINOR) };
               ImGui_ImplOpenGL3_Init(openglVersion.c_str());
           }
       }
       catch (const std::bad_any_cast& exception) {
           KATE_APP_LOGGER_CRITICAL("Exception thrown std::any_cast. What: {}", exception.what());
       }
   }

   auto ImGuiLayer::OnDetach() -> void {
       ImGui_ImplOpenGL3_Shutdown();
       ImGui_ImplGlfw_Shutdown();
       ImGui::DestroyContext();
   }

   auto ImGuiLayer::OnUpdate() -> void {

   }


   auto ImGuiLayer::BeginFrame() -> void {
       ImGui_ImplOpenGL3_NewFrame();
       ImGui_ImplGlfw_NewFrame();
       ImGui::NewFrame();
   }

   auto ImGuiLayer::EndFrame() -> void {
       ImGuiIO& io{ ImGui::GetIO() };
       Window& window{ Application::Get().GetMainWindow() };
       io.DisplaySize = ImVec2(window.GetWidth(), window.GetHeight());

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

   auto ImGuiLayer::OnImGuiRender() -> void {

   }

   auto ImGuiLayer::OnEvent(Event &event) -> void {
       if (m_BlockEvents) {
           ImGuiIO& io{ ImGui::GetIO() };
           // To be revised: ImGuiLayer would not propagate mouse events and key events when and ImGui item captures them
           event.SetHandled(event.IsInCategory(MOUSE_EVENT_CATEGORY) && io.WantCaptureMouse);
           event.SetHandled(event.IsInCategory(KEY_EVENT_CATEGORY) && io.WantCaptureKeyboard);
       }
   }

   ImGuiLayer::~ImGuiLayer() = default;
}