#pragma once
#include <string>
#include <map>
#include<vector>
#include<string>
#include<sstream>
#include<fstream>

using namespace std;
namespace ALStore {
    class Product
    {
    private:
        int id;
        string name;
        string category;
        double price;
        int quantity;

    public:
        Product(int id, string name, string category, double price, int quantity)
        {
            this->id = id;
            this->name = name;
            this->category = category;
            this->price = price;
            this->quantity = quantity;
        }
       
        int getId() const { return id; }
        void setId(int id) { this->id = id; }
        string getName() const { return name; }
        void setName(string name) { this->name = name; }
        string getCategory() const { return category; }
        void setCategory(string category) { this->category = category; }
        double getPrice() const { return price; }
        void setPrice(double price) { this->price = price; }
        int getQuantity() const { return quantity; }
        void setQuantity(int quantity) { this->quantity = quantity; }
    };
    struct Inventory{
        int id; string name; double price; string category; int invQuantity;
    }; //Inventory m_Stock;
    class StoreInventory
    {
    public:
        StoreInventory();
        ~StoreInventory();
        void CreateInventory(); 
        void UpdateInventory(string name, double price, string category, int invQuantity);
        void loadInventoryFromFile(std::string filename);
        void print();
        bool getSale() { return m_Sell; }
        void setSale(bool s) { m_Sell = s; }
        int getSold() { return m_Sold; }
        Product* FindProduct(int id);
        void UpdateSales(int id);
        vector<Inventory> GetInventory() const { return storeInv; }
        vector<Inventory>& GetInventory() { return storeInv; }
        void updateProduct(int id, string name, string category, double price, int quantity);
        vector<Product> venProducts, storeProducts;
        int productSize() { return venProducts.size(); }
  //      friend class Engine;
    private:
        bool m_Sell = false;
        int m_Sold = 0;
        vector<Inventory> storeInv;
    };
}