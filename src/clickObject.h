#ifndef CLICKOBJECT_H
#define CLICKOBJECT_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "texture.h"
#include "spriteRenderer.h"
#include "gameObject.h"

class ClickObject : public GameObject
{
public:
    // click attributes state
    float Radius;
    float TimeAlive;

    // constructor(s)
    ClickObject();
    ClickObject(glm::vec2 pos, 
        float radius,
        float timeAlive,
        Texture2D sprite);
    
    void Move(float x, float y);
    void Enable();
    void Disable();
};

#endif