#include "mypch.h"
#include "Engine.h"
#include "core/KeyCodes.h"

#define BIND_EVENT_FN(x) std::bind(&Engine::x, this, std::placeholders::_1)
Engine* Engine::s_Instance = nullptr;
Engine::Engine() : m_Camera(1.0f, 1.0f, 1.0f, 1.0f) { s_Instance = this; }
Engine::~Engine() {}
static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}
bool Engine::init() {
    m_Window = std::make_unique<ALStore::Window>();
    m_TextureLibrary = std::make_unique<ALStore::TextureLibrary>();
    m_Renderer = std::make_unique<ALStore::Render2D>();
    m_ImGui = std::make_unique<ALStore::ImGuiLayer>();
    m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
    AL_INFO("Window init");
    m_Window->init(1400, 800);
    m_Window->setRunning(true);
    m_TextureLibrary->init();
    m_TextureLibrary->Load("assets/textures/Truck2.png");
    m_texture = m_TextureLibrary->Get("Truck2.png");
    m_Renderer->Init();
    if (!m_ImGui->init()) AL_CORE_ERROR("ImGui failure!");
 
	return true;
}
void Engine::OnEvent(Event& e)
{
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
    dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(OnMouseButtonPressedEvent));
    dispatcher.Dispatch<MouseButtonReleasedEvent>(BIND_EVENT_FN(OnMouseButtonReleasedEvent));
    dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FN(OnMouseMovedEvent));
    dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(OnMouseScrolledEvent));
    dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(OnKeyPressedEvent));
    //dispatcher.Dispatch<KeyTypedEvent>(BIND_EVENT_FN(OnKeyTypedEvent));
    //dispatcher.Dispatch<KeyReleasedEvent>(BIND_EVENT_FN(OnKeyReleasedEvent));
    dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResizeEvent));
    //AL_CORE_TRACE("{0}", e);
}
void Engine::run() { 
    m_Camera.SetProjection(-2, 2, -2, 2);
    while (m_Window->isRunning())
    {
        m_Renderer->BeginScene(m_Camera);
        m_ImGui->begin();
   
        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            //ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }
        // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }

        // Rendering

        int display_w, display_h;
        
        glfwGetFramebufferSize(m_Window->GetNativeWindow(), &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        m_texture->Bind();
        glm::mat4 transform2 = ALStore::Maths::getTransform(m_size, m_position);
        m_Renderer->DrawTexture(transform2, m_texture, { 1.0f, 1.0f, 1.0f, 1.0f });
        m_Renderer->EndScene();
        m_ImGui->render();
        m_Window->OnUpdate();
    }
}
bool Engine::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e) {
    //HZ_PROFILE_FUNCTION("MousePicking");
    if (e.GetMouseButton() == 0) {
        AL_INFO("MOUSE PRESSED{0}", e.GetMouseButton());
    }
    return false;
}
bool Engine::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e) { return false; }
bool Engine::OnMouseMovedEvent(MouseMovedEvent& e) {
    m_mousePos.x = e.GetX(); m_mousePos.y = e.GetY();//AL_CORE_INFO("{0}, {1}", m_mousePos.x, m_mousePos.y);
    return false;
}
bool Engine::OnMouseScrolledEvent(MouseScrolledEvent& e) {
    float y = e.GetYOffset(); AL_CORE_TRACE("Engine: zoom ={0}", m_ZoomLevel);
    m_ZoomLevel = m_ZoomLevel + e.GetYOffset();
    m_ZoomLevel -= e.GetYOffset() * 0.25f;
    m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
    m_ZoomLevel = std::min(m_ZoomLevel, 14.25f);
    m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
    return true;
}
bool Engine::OnKeyPressedEvent(KeyPressedEvent& e) {
    //AL_CORE_TRACE("Engine: key pressed");
    KeyPressedEvent& eventK = (KeyPressedEvent&)e;
    int ts = 1;
    glm::vec3 pos = m_Camera.GetPosition();
    switch (eventK.GetKeyCode()) {
    case HZ_KEY_TAB: AL_TRACE("Tab key is pressed (event)!"); break;
    case HZ_KEY_A: pos.x -= m_CameraTranslationSpeed * ts; break;
    case HZ_KEY_D: pos.x += m_CameraTranslationSpeed * ts; break;
    case HZ_KEY_W: pos.y += m_CameraTranslationSpeed * ts; break;
    case HZ_KEY_S: pos.y -= m_CameraTranslationSpeed * ts; break;
    default:
        break;
    }
    m_Camera.SetPosition(pos);
    //HZ_CORE_TRACE("Engine: New m_CameraPosition ={0}",  glm::to_string(pos) );
    //m_CameraTranslationSpeed = m_ZoomLevel;	//HZ_TRACE("{0}", (char)e.GetKeyCode());
    return false;
}
bool Engine::OnWindowResizeEvent(WindowResizeEvent& e) {
    if (e.IsInCategory(EventCategoryApplication))
    {
        AL_TRACE(e);
    }
    if (e.IsInCategory(EventCategoryInput))
    {
        AL_TRACE(e);
    }
    if (e.GetWidth() == 0 || e.GetHeight() == 0)
    {
        m_Minimized = true;
        return false;
    }

    m_Minimized = false;
    //m_Framebuffer->Resize(e.GetWidth(), e.GetHeight());
   // draw.OnWindowResize(e.GetWidth(), e.GetHeight()); //glViewport

    return false;

}
bool Engine::OnWindowClose(WindowCloseEvent& e)
{
    AL_TRACE("Goodbye");

    m_Window->setRunning(false);
    return true;
}