#pragma once
#include <stdio.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h> 
#include "glad/include/glad/glad.h"
#include "glm/glm/glm.hpp"
#include "glm/glm/ext.hpp"
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "implot/implot.h"
#include "implot/implot_internal.h"
#include "window/Window.h"
#include "events/Event.h"
#include "events/ApplicationEvent.h"
#include "events/MouseEvent.h"
#include "events/KeyEvent.h"
#include "render/OrthographicCamera.h"
#include "storeLayer/StoreClass.h"

using namespace ALStore;
class Engine
{
public:
	Engine();
	~Engine();
	bool init();
	void run();
	static Engine& Get() { return *s_Instance; }
	ALStore::Window& GetWindow() { return *m_Window; }

private:
	//GLFWwindow* m_Window;
	std::unique_ptr<ALStore::Window> m_Window;
	std::unique_ptr<StoreClass> m_Store;

	bool show_demo_window = true;
	bool show_another_window = false; 
	GLFWwindow* s_Window; int ww, wh;
	glm::vec4 clear_color = glm::vec4(0.45f, 0.55f, 0.60f, 1.00f);
	glm::vec2 m_mousePos = { .75f, .75f };
	bool m_Minimized = false;
	ALStore::OrthographicCamera m_Camera;
	float m_AspectRatio = 1.0f;
	float m_ZoomLevel = 1.25f;
	float m_CameraTranslationSpeed = .2250f, m_CameraRotationSpeed = 180.0f;
	void OnEvent(Event& e);

	bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
	bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
	bool OnMouseMovedEvent(MouseMovedEvent& e);
	bool OnMouseScrolledEvent(MouseScrolledEvent& e);
	bool OnKeyPressedEvent(KeyPressedEvent& e);
	//bool OnKeyReleasedEvent(KeyReleasedEvent& e);
	//bool OnKeyTypedEvent(KeyTypedEvent& e);
	bool OnWindowResizeEvent(WindowResizeEvent& e);
	bool OnWindowClose(WindowCloseEvent& e);
	static Engine* s_Instance;
};

