#pragma once
#include <stdio.h>
#include <Windows.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h> 
#include "glad/include/glad/glad.h"
#include "glm/glm/glm.hpp"
#include "glm/glm/ext.hpp"
#include "window/Window.h"
#include "core/Timestep.h"
#include "events/Event.h"
#include "events/ApplicationEvent.h"
#include "events/MouseEvent.h"
#include "events/KeyEvent.h"
#include "render/OrthographicCamera.h"
#include "imgui/ImGuiLayer.h"
#include "render/TextureLibrary.h"
#include "render/Render2D.h"
#include "render/Shader.h"
#include "render/Maths.h"
#include "render/Framebuffer.h"
#include "scene/Scene.h"
#include "scene/Entity.h"
#include "scene/XMLClass.h"
#include "Delivery.h"
#include "StoreInventory.h"
using namespace ALStore;

	class Engine
	{
	public:
		Engine();
		~Engine();
		bool init();
		void run();
		void update();
		static Engine& Get() { return *s_Instance; }
		ALStore::Window& GetWindow() { return *m_Window; }
		
		//Scene& GetScene() { return *m_ActiveScene; }

	private:
		//GLFWwindow* m_Window;
		std::unique_ptr<ALStore::Window> m_Window;
		std::unique_ptr<ALStore::TextureLibrary> m_TextureLibrary;
		std::shared_ptr<ALStore::Render2D> m_Renderer;
		std::unique_ptr<ALStore::ImGuiLayer> m_ImGui;
		std::shared_ptr<ALStore::Texture2D> m_$texture, m_StoreTxt, m_Truck2Txt;
		std::shared_ptr<ALStore::Scene> m_ActiveScene;
		std::shared_ptr<ALStore::Shader> fbShader;
		std::shared_ptr<ALStore::OpenGLFramebuffer> m_Framebuffer; ALStore::FramebufferSpecification fbSpec;
		std::shared_ptr<Delivery> trkDelivery;
		StoreInventory storeInv; 
		StoreInventory* m_Inventory_ptr;
		std::shared_ptr<ALStore::StoreInventory> m_Inventory;
		void storeClicked(); bool m_StorePanel = false;
		bool show_demo_window = true;
		bool show_another_window = false;
		//GLFWwindow* s_Window; int ww, wh;
		glm::vec4 clear_color = glm::vec4(0.45f, 0.55f, 0.60f, 1.00f);
		//glm::vec2 m_mousePos = { .75f, .75f };
		bool m_Minimized = false;
		Timestep ts;
		double const limitFPS = 1.0 / 60.0;
		double lastTime = glfwGetTime(), timer = lastTime;
		double deltaTime = 0, nowTime = 0;
		int frames = 0, updates = 0;
		bool $out, $in;
		bool renTruck = true;
		void render();
		ALStore::OrthographicCamera m_Camera;
		float m_AspectRatio = 1.0f;
		float m_ZoomLevel = 6.5f;
		float m_CameraTranslationSpeed = .2250f, m_CameraRotationSpeed = 180.0f;
		glm::vec2 store_size = { .402f, .750f }; glm::vec3 store_position = { -2.9f, 0.5f, 0.0f };
		glm::vec2 m_size = { .75f, .75f };
		glm::vec3 m_position = { 0.5f, 0.5f, 0.0f };
		glm::vec4 m_colorG = { 0.0f, 1.0f, 0.0f, 1.0f };
		glm::vec4 m_colorR = { 1.0f, 0.0f, 0.0f, 1.0f };
		Entity e_$ign, e_Store, e_Truck2;
		Entity m_HoveredEntity;
		glm::vec2 truck2_size = { .451f, .750f };
		glm::vec3 truck2_position = { 1.2f, 0.041f, 0.0f };
		bool m_Flipped = false;
		entt::entity  m_SquareEntity;
		XMLClass XMLClass;
		void ImGuiEdit(Entity entt);
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
