#include "mypch.h"
#include "Engine.h"
#include "ImGuiLayer.h"


namespace ALStore {
    ImGuiLayer::ImGuiLayer() {
        //int* quantity = new int[np];  ////////////////move
        
    }
    ImGuiLayer::~ImGuiLayer() {}
    bool ImGuiLayer::init() {
       // m_Products = std::make_shared<Product>();
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
        m_Products.reset(new StoreInventory());
        m_Products->CreateInventory();
        m_Products->loadInventoryFromFile("assets/Inventory.csv");
        int np = m_Products->productSize();
        // ImGui::InputInt("Tea @ $" + prices[1], &tea);

        std::ostringstream os;
        for (size_t i = 0; i < np; i++)
        {
            os.str("");
            Product* item = m_Products->FindProduct(i + 1);
            name[i] = item->getName();
            purQuantity[i] = item->getQuantity();
            price[i] = item->getPrice();

            os << name[i] << ": $" << price[i];
            str[i] = os.str();
          //  c[i] = str.c_str();
        }

//        for (int j = 0; j < 3 + 1; j++) { quantity[j] = 0; }
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
    void ImGuiLayer::render() {bool closer = true; static int clicked = 0;
        ImGui::Begin("Inventory", &closer);
        ImGui::SeparatorText("Purchase");  
        for (size_t i = 0; i < 4; i++){
            ImGui::PushItemWidth(80);
            ImGui::InputInt(str[i].c_str(), &purQuantity[i]);
        }
        ImGui::SeparatorText("Inventory");
        vector<Inventory> inv = m_Products->GetInventory();
        std::ostringstream os;
        for (auto& row : inv)        {
            os.str("");
            string name = row.name;
            int t = row.invQuantity;
            os << name << ": " << t;
            std::string s; s = os.str();
            const char* c = s.c_str();
            ImGui::PushItemWidth(80);
            ImGui::Text(c);
        }

        if (ImGui::Button("Buy"))
            clicked++;
            double total = 0;
            if (clicked & 1){ 
                int p = 0;
                for (auto& row : inv){
                    row.invQuantity = row.invQuantity + purQuantity[p];
                    m_Products->UpdateInventory(row.name, row.price, row.category, row.invQuantity);
                    total += purQuantity[p] * price[p];
                    p++;
                }    
            AL_INFO("Purchased ${0}", total);
            ImGui::SameLine();
            ImGui::Text("Order processed."); clicked = 0;
        }
  
        ImGui::End();
        static bool show = true; ImGui::ShowDemoWindow(&show);
    }

    void ImGuiLayer::end() {
        //HZ_PROFILE_FUNCTION();
        ImGuiIO& io = ImGui::GetIO();
        capMouse = io.WantCaptureMouse;
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