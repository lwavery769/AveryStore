#pragma once
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
		void end();
		bool GetIMC() { return capMouse; }
		void SetStoreClose(bool x) { m_closed = x; }
		GLFWwindow* s_Window; int ww, wh;
		string name[4];
		double price[4];
		int purQuantity[4];
	private:
		bool show_demo_window = true;
		bool show_another_window = false;
		bool capMouse; 
		bool m_closed; 
		std::shared_ptr<ALStore::StoreInventory> m_Products;
		int items = 4;
		std::string str[4];
		const char* c[4];
		Product* FindProduct(int id); 
		std::vector<int> invQuantity;
	};
}