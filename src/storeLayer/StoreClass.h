#pragma once
#include <string>
#include "ImGui/imgui.h"
#include "ImGui/backends/imgui_impl_glfw.h"
#include "ImGui/backends/imgui_impl_opengl3.h"
#include "implot/implot.h"
#include "implot/implot_internal.h"
using namespace std;
class Product
{
private:
    int id;
    string name;
    string category;
    double cost, price;
    int quantity, stock;

public:
    Product(int id, string name, string category, double cost, int quantity)
    {
        this->id = id;
        this->name = name;
        this->category = category;
        this->cost = cost;
        this->price = cost + 1.00;
        this->quantity = quantity;
        this->stock = 0;
    }

    int getId() const { return id; }
    void setId(int id) { this->id = id; }
    string getName() const { return name; }
    void setName(string name) { this->name = name; }
    string getCategory() const { return category; }
    void setCategory(string category) { this->category = category; }
    double getCost() const { return cost; }
    double getPrice() const { return price; }
    void setPrice(double price) { this->price = price; }
    int getQuantity() const { return quantity; }
    void setQuantity(int quantity) { this->quantity = quantity; }
    int getStock() const { return stock; }
    void setStock(int stock) { this->stock = stock; }
};
class StoreClass
{
public:
	StoreClass();
	~StoreClass();
	void loadInventoryFromFile(std::string filename);
	void print();
    void drawLayer();
private:
    vector<Product> venProducts; int purQuantity[4];
    std::string str[4];
};