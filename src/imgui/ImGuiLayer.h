#pragma once
namespace ALStore {
	class ImGuiLayer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();
		bool init();
		void begin();
		void OnDetach();
		void render();
		void end();
GLFWwindow* s_Window; int ww, wh;
	private:
		bool show_demo_window = true;
		bool show_another_window = false;
	};
}