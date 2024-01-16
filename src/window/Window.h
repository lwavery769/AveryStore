#pragma once
#include "glad/glad.h"
#include "glfw/glfw3.h"
#include "Events/Event.h"

namespace ALStore {
	class Window
	{
	public:
		Window();
		~Window();
		struct WindowProps
		{
			std::string Title;
			uint32_t Width;
			uint32_t Height;

			WindowProps(const std::string& title = "Odyssey Engine",
				uint32_t width = 1280,
				uint32_t height = 720)
				: Title(title), Width(width), Height(height)
			{
			}
		};
		bool init(int w, int h);
		void OnUpdate();
		void OnShutdown();
		void setRunning(bool r) { m_running = r; }
		bool isRunning() { return m_running; }
		inline GLFWwindow* GetNativeWindow() const { return m_Window; }
		void* GetWindow() { return m_Window; }
		int GetWidth() { return m_Data.Width; }
		int GetHeight() { return m_Data.Height; }
		void showFPS(GLFWwindow* pWindow);
		using EventCallbackFn = std::function<void(Event&)>;
		inline void SetEventCallback(const EventCallbackFn& callback) { m_Data.EventCallback = callback; }
		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	private:
		bool m_running = true;
		bool m_Minimized = false;
		int width, height;
		double lastTime, nbFrames;
		float m_LastFrameTime = 0.0f;
		GLFWwindow* m_Window;
	
	};

}