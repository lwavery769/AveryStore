#include "mypch.h"
#include "Engine.h"
#include "scene/Componets.h"
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
    //m_ActiveScene = std::make_shared<ALStore::Scene>();
    m_ImGui = std::make_unique<ALStore::ImGuiLayer>();
    m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
    AL_INFO("Window init");
    m_Window->init(1400, 800);
    m_Window->setRunning(true);

    m_TextureLibrary->init();
    m_TextureLibrary->Load("assets/textures/Truck2.png");
    m_texture = m_TextureLibrary->Get("Truck2.png");

    m_ActiveScene.reset(new Scene());
    m_Store = m_ActiveScene->CreateEntity("store1");
    m_Store.AddComponent<IDComponent>(13466807492172565454);  
    m_StoreTxt = std::make_shared<Texture2D>("assets/Textures/Store.png");
    m_Store.AddComponent<SpriteRendererComponent>(m_StoreTxt);
    

    m_position = { -1.533f, 0.762f, 0.0f };
    m_size = { 0.432f, 1.096f };
    m_Store.GetComponent<TransformComponent>().Transform = Maths::getTransform(m_size, m_position);
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
        // Rendering

        int display_w, display_h;
        
        glfwGetFramebufferSize(m_Window->GetNativeWindow(), &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        m_texture->Bind();
        glm::mat4 transform2 = ALStore::Maths::getTransform(m_size, m_position);
        m_Renderer->DrawTexture(transform2, m_texture, { 1.0f, 1.0f, 1.0f, 1.0f });
        
        m_Store.GetComponent<SpriteRendererComponent>().Texture->Bind();
        m_Renderer->DrawSprite(m_Store.GetComponent<TransformComponent>().Transform, m_Store.GetComponent<SpriteRendererComponent>(), m_Store.GetComponent<IDComponent>().ID);
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