

#include "bubbleObject.h"


BubbleObject::BubbleObject() 
    : GameObject(), Points(1.0f) ,Radius(1.0f){ }

BubbleObject::BubbleObject(glm::vec2 pos,
    float radius,
    float points,
    glm::vec2 velocity,
    Texture2D sprite)
: GameObject(pos, 
    glm::vec2(radius * 2.0f, radius * 2.0f), 
    sprite, glm::vec3(1.0f),
    velocity),
    Points(points), 
    Radius(radius){ }