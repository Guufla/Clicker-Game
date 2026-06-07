

#include "bubbleObject.h"


extern AudioManager *Audio;

BubbleObject::BubbleObject() 
    : GameObject(), Points(1.0f) ,Radius(1.0f),PopTime(2.0f),PopTimer(0.0f),IsPopping(false){ }

BubbleObject::BubbleObject(glm::vec2 pos,
    float       radius,
    float       points,
    float       popTime,
    glm::vec2   velocity,
    Texture2D   sprite,
    bool        centerPivot,
    glm::vec3 colliderShape)
: GameObject(pos, 
    glm::vec2(radius * 2.0f, radius * 2.0f), 
    sprite, 
    centerPivot, 
    colliderShape,
    velocity),
    Points(points), 
    Radius(radius),
    PopTime(popTime),
    PopTimer(0.0f),
    IsPopping(false){ }

void BubbleObject::Update(float dt)
{
    if(IsPopping)
    {
        PopTimer += dt;
        Sprite = ResourceManager::GetTexture("popFrame1");
        if(PopTimer >= PopTime)
        {
            IsPopping = false;
            this->Destroyed = true;
            if(Audio != nullptr)
            {
                Audio->PlayPopSound();
            }
        }
        else if(PopTimer > PopTime*0.7f){
            Sprite = ResourceManager::GetTexture("popFrame3");
        }
        else if(PopTimer > PopTime*0.4f)
        {
            Sprite = ResourceManager::GetTexture("popFrame2");
            
        }
    }
}

void BubbleObject::CollisionDetected(GameObject &other)
{
    // If the bubble collides with the click object, destroy the bubble and add points to the player
    if(other.Tag==2 && this->IsPopping == false)
    {
        this->IsPopping = true;
        this->Velocity = glm::vec2(0.0f);
    }
    else if(other.Tag==0)
    {
        if(other.ColliderShape.x > other.ColliderShape.y)
        {
            //this->Velocity.x = -this->Velocity.x;
            this->Velocity.y = -this->Velocity.y;
        }
        else
        {
            this->Velocity.x = -this->Velocity.x;
            //this->Velocity.y = -this->Velocity.y;
        }
    }
}