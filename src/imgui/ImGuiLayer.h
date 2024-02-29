#pragma once
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
//#include "ImPlot/implot.h" 
//#include "ImPlot/implot_internal.h"
#include "../StoreInventory.h"
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
		void renderTabs();
		void updateSales(double timer);
		void end();
		bool GetIMC() { return capMouse; }
		void StoreClick();
		GLFWwindow* s_Window; int ww, wh;
		string name[4];
		double price[4];
		int purQuantity[4];
	private:
		bool show_demo_window = true;
		bool show_another_window = false;
		bool capMouse; 
		std::shared_ptr<ALStore::StoreInventory> m_Products;
		int items = 4;
		std::string str[4];
		const char* c[4];
		Product* FindProduct(int id); 
		std::vector<int> invQuantity;
	};
}