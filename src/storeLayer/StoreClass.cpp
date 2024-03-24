#include "mypch.h"
#include <sstream>
#include "StoreClass.h"
StoreClass::StoreClass() : purQuantity{ 0,0,0,0 } {}
StoreClass::~StoreClass() {}
void StoreClass::loadInventoryFromFile(std::string filename) {
    std::ifstream file;
    file.open(filename);

    if (file.is_open())
    {
        std::string line;
        while (getline(file, line))
        {
            std::stringstream ss(line);
            std::string idStr, name, category, priceStr, quantityStr;
            getline(ss, idStr, ',');
            getline(ss, name, ',');
            getline(ss, priceStr, ',');
            getline(ss, category, ',');
            getline(ss, quantityStr, ',');

            int id = stoi(idStr);
            double price = stod(priceStr);
            int quantity = stoi(quantityStr);

            Product p(id, name, category, price, quantity);
            venProducts.push_back(p);
        }
        file.close();
        int np = venProducts.size();
        std::ostringstream os;
        for (size_t i = 0; i < np; i++)
        {
            os.str("");
            os << venProducts[i].getName() << ": $" << venProducts[i].getPrice();
            str[i] = os.str();
        }
        print();
    }
    else {
        AL_WARN("Error: Could not open file {0}", filename);
    }
}
void StoreClass::print() {
    for (auto i = venProducts.begin(); i != venProducts.end(); i++)
    {
        AL_INFO("ID : {0}", i->getId());
        AL_INFO("Name : {0}", i->getName());
        AL_INFO("Category : {0}", i->getCategory());
        AL_INFO("Price : ${0}", i->getPrice());
        AL_INFO("Quantity : {0}", i->getQuantity());
    }
}
void StoreClass::drawLayer() {
   
    ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
    if (ImGui::BeginTabBar("Store Operations", tab_bar_flags)) {
        if (ImGui::BeginTabItem("Vendor")) {
            ImGui::SeparatorText("Purchase");
            for (size_t i = 0; i < 4; i++) {
                //purQuantity[i] = venProducts[i].getQuantity();
                ImGui::PushItemWidth(80);
                ImGui::InputInt(str[i].c_str(), &purQuantity[i] );
            }
            static int clicked = 0;
            if (ImGui::Button("Buy"))
                clicked++;
            double total = 0;
            if (clicked & 1) {
                int p = 0;
                for (auto& row : inv) {
                    row.invQuantity = row.invQuantity + purQuantity[p];
                    m_Products->UpdateInventory(row.name, row.price, row.category, row.invQuantity);
                    total += purQuantity[p] * price[p];
                    p++;
                }
                AL_INFO("Purchased ${0}", total);
                ImGui::SameLine();
                ImGui::Text("Order processed."); clicked = 0;
                //engine.SetTrk(true);

            }
            ImGui::EndTabItem();
            /*
            std::ostringstream os;
        if (ImGui::BeginTabItem("Inventory")) {
            ImGui::SeparatorText("Inventory");
            for (size_t i = 0; i < 4; i++) {
                int q = venProducts[i].getStock();
                ImGui::PushItemWidth(80);
                ImGui::InputInt(str[i].c_str(), &purQuantity[i]);
            }
            ImGui::EndTabItem();
        }
        /*   ImGui::SeparatorText("Inventory");
            vector<Inventory> inv = m_Products->GetInventory();
            std::ostringstream os;
            for (auto& row : inv) {
                os.str("");
                string name = row.name;
                int t = row.invQuantity;
                os << name << ": " << t;
                std::string s; s = os.str();
                const char* c = s.c_str();
                ImGui::PushItemWidth(80);
                ImGui::Text(c);
            }
            static int clicked = 0;
            if (ImGui::Button("Buy"))
                clicked++;
            double total = 0;
            if (clicked & 1) {
                int p = 0;
                for (auto& row : inv) {
                    row.invQuantity = row.invQuantity + purQuantity[p];
                    m_Products->UpdateInventory(row.name, row.price, row.category, row.invQuantity);
                    total += purQuantity[p] * price[p];
                    p++;
                }
                AL_INFO("Purchased ${0}", total);
                ImGui::SameLine();
                ImGui::Text("Order processed."); clicked = 0;
                //engine.SetTrk(true);

            }
            ImGui::EndTabItem();
        }
        std::ostringstream os;
        if (ImGui::BeginTabItem("Inventory")) {
            ImGui::SeparatorText("Inventory");
            vector<Inventory> inv = m_Products->GetInventory();

            for (auto& row : inv) {
                os.str("");
                string name = row.name;
                int t = row.invQuantity;
                os << name << ": " << t;
                std::string s; s = os.str();
                const char* c = s.c_str();
                ImGui::PushItemWidth(80);
                ImGui::Text(c);
            }
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Sales")) {
            os.str("");
            os << "Product sold" << ": $" << m_Products->getSold();
            std::string s; s = os.str();
            const char* c = s.c_str();
            ImGui::PushItemWidth(80);
            ImGui::Text(c);
            ImGui::EndTabItem();
        }*/
        ImGui::EndTabBar();
        }
    }
}