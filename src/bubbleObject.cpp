

#include "bubbleObject.h"


BubbleObject::BubbleObject() 
    : GameObject(), Points(1.0f) ,Radius(1.0f){ }

BubbleObject::BubbleObject(glm::vec2 pos,
    float       radius,
    float       points,
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
    Radius(radius){ }
    
    
void BubbleObject::CollisionDetected(GameObject &other)
{
    // If the bubble collides with the click object, destroy the bubble and add points to the player
    if(other.Tag==2)
    {
        PopBubble();
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

// Implement Code to pop the bubble which increases the score counter based on the amount of points in the points
void BubbleObject::PopBubble()
{
    // This is where you would implement the pop code
    this->Destroyed = true;
}