#include "mypch.h"
#include "Engine.h"
#include "ImGuiLayer.h"
namespace ALStore {
    ImGuiLayer::ImGuiLayer() {  }
    ImGuiLayer::~ImGuiLayer() {}
    bool ImGuiLayer::init() {
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsLight();

        Engine& app = Engine::Get();
        s_Window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());
        ww = app.GetWindow().GetWidth(); wh = app.GetWindow().GetHeight();
        // Setup Platform/Renderer bindings
        ImGui_ImplGlfw_InitForOpenGL(s_Window, true);
        ImGui_ImplOpenGL3_Init("#version 130");
        return true;
    }
    void ImGuiLayer::OnDetach()
    {
        //  HZ_PROFILE_FUNCTION();
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
    void ImGuiLayer::begin() {        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }
    void ImGuiLayer::render() {
        static bool show = true;
        ImGui::ShowDemoWindow(&show);
    }
    void ImGuiLayer::end() {
        //HZ_PROFILE_FUNCTION();
        ImGuiIO& io = ImGui::GetIO();
        Engine& app = Engine::Get();
        s_Window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());
        ww = app.GetWindow().GetWidth(); wh = app.GetWindow().GetHeight();

        io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

        // Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        ImGui::EndFrame();
        /*   if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
            {
                GLFWwindow* backup_current_context = glfwGetCurrentContext();
                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault();
                glfwMakeContextCurrent(backup_current_context);
            }
        */
    }
}