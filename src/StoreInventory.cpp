#include "StoreInventory.h"
#include "core/Log.h"

namespace ALStore {
    StoreInventory::StoreInventory() : m_Sell(true)
    {}
    StoreInventory::~StoreInventory() {}
    void StoreInventory::CreateInventory() {
          string name; double price = 0.0; string category; int invQuantity = 0;
          
          
          struct Inventory A[] = { {1, "Sugar", 1.00, "Staples", 10 },
                               { 2, "Tea", 2.00, "Spices", 7},
                               { 3, "Spice", 2.00, "Staples", 5},
                                { 4, "Milk", 3.5, "Dairy", 3}};
          vector<Inventory> v;
          // Insertion of elements using push_back()
          for (int i = 0; i < sizeof(A) / sizeof(A[0]); i++) {
                  Inventory stock;
            stock.name = A[i].name;
            stock.price = A[i].price;
            stock.category = A[i].category;
            stock.invQuantity = A[i].invQuantity; 
            storeInv.push_back(A[i]);
          }
    }
    void StoreInventory::loadInventoryFromFile(std::string filename) {
        ifstream file;
        file.open(filename);

        if (file.is_open())
        {
            std::string line;
            while (getline(file, line))
            {
                stringstream ss(line);
                string idStr, name, category, priceStr, quantityStr;
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
            print();
        }
        else {
            AL_WARN("Error: Could not open file {0}", filename);
        }
    }    
    void StoreInventory::UpdateInventory(string name, double price, string category, int invQuantity) {
        bool found = false;
        for (auto i = storeInv.begin(); i != storeInv.end(); i++){
            if (i->name == name){
                i->price = price;i->category = category;i->invQuantity = invQuantity;
            }
        }
    }
    void StoreInventory::UpdateSales(int id) {
        bool found = false;
        for (auto i = storeInv.begin(); i != storeInv.end(); i++) {
            if (i->id == id) {
                i->invQuantity = i->invQuantity - 1; AL_INFO("1 sold");
                m_Sold = m_Sold + i->price;
            }
        }
    }
    void StoreInventory::print() {
        for (auto i = storeProducts.begin(); i != storeProducts.end(); i++)
        {
            AL_INFO("ID : {0}", i->getId());
            AL_INFO("Name : {0}", i->getName());
            AL_INFO("Category : {0}", i->getCategory());
            AL_INFO("Price : ${0}", i->getPrice());
            AL_INFO("Quantity : {0}", i->getQuantity());
        }
    }
    Product* StoreInventory::FindProduct(int id) {
        for (auto i = venProducts.begin(); i != venProducts.end(); i++){
            if (i->getId() == id){return &(*i);}
        }
        return nullptr;

    }
    void StoreInventory::updateProduct(int id, string name, string category, double price, int quantity) {
        bool found = false;
        for (auto i = storeProducts.begin(); i != storeProducts.end(); i++)
        {
            if (i->getId() == id){
                i->setName(name);
                i->setCategory(category);
                i->setPrice(price);
                i->setQuantity(quantity);
                found = true;
                break;
            }
        }
        if (!found) { AL_ERROR("ID does not exist."); }
    }
}