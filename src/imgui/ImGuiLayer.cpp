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
void ImGuiLayer::begin() {        // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}
void ImGuiLayer::render() {
    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
void ImGuiLayer::end() { ImGui::End(); }
}