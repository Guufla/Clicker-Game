#include "clickObject.h"

ClickObject::ClickObject() 
    : GameObject(), Radius(1.0f), TimeAlive(0.0f){ }
    
ClickObject::ClickObject(glm::vec2 pos,
    float radius,
    float timeAlive,
    Texture2D sprite)
: GameObject(pos,
    glm::vec2(radius * 2.0f, radius * 2.0f), 
    sprite, glm::vec3(1.0f)),
    Radius(radius), 
    TimeAlive(timeAlive){ }

void ClickObject::Move(float x, float y)
{
    this->Position.x = x;
    this->Position.y = y;
}
void ClickObject::Enable()
{
    this->IsDisabled = false;
}
void ClickObject::Disable()
{
    this->IsDisabled = true;
}