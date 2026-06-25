#ifndef MENUOBJECT_H
#define MENUOBJECT_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "texture.h"
#include "spriteRenderer.h"
#include "gameObject.h"
#include "resourceManager.h"
#include "audioManager.h"


#include <iostream>




// Container object for holding all state relevant for a single
// game object entity. Each object in the game likely needs the
// minimal of state as described within GameObject.
class MenuObject : public GameObject
{
public:
    // Menu state
    glm::vec2 pos;
    float Size;
    
    // constructor(s)
    MenuObject();
    MenuObject(glm::vec2 pos, float size, float points, float popTime, glm::vec2 velocity, Texture2D sprite, bool centerPivot, glm::vec3 colliderShape);

    void Update(float dt);
    // Functions
    void CollisionDetected(GameObject& other) override;
    void PopBubble();
};

#endif