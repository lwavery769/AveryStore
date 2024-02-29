#include "Delivery.h"
Delivery::Delivery() : m_Deliver(true)
{
    m_DelTruckTxt = std::make_shared<ALStore::Texture2D>("assets/Textures/Truck2.png");
}
Delivery::~Delivery() {}
void Delivery::init() {


}
void Delivery::updatePosition() {
    if (m_Flipped) {
        truck2_position.x = truck2_position.x + .0075f;
        if (truck2_position.x < -0.9974995f)
        {
        }
        if (truck2_position.x > 1.237f) {
            m_Flipped = false;
        }
    }
    else {
        truck2_position.x = truck2_position.x - .0075f;
        if (truck2_position.x < -2.873f) {
            m_Flipped = true;
        }
    }
}
void Delivery::Render() {
    m_DelTruckTxt->Bind();
    draw.DrawTexture(ALStore::Maths::getTransform(truck2_size, truck2_position), m_DelTruckTxt, m_TintColor, 22, m_Flipped);
}
