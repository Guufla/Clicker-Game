#ifndef BUBBLEOBJECT_H
#define BUBBLEOBJECT_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "texture.h"
#include "spriteRenderer.h"
#include "gameObject.h"


// Container object for holding all state relevant for a single
// game object entity. Each object in the game likely needs the
// minimal of state as described within GameObject.
class BubbleObject : public GameObject
{
public:
    // bubble state
    float Points;
    float Radius;

    // constructor(s)
    BubbleObject();
    BubbleObject(glm::vec2 pos, float radius, float points, glm::vec2 velocity, Texture2D sprite,bool centerPivot, glm::vec3 colliderShape);
    // Functions
    void CollisionDetected(GameObject& other) override;
    void PopBubble();
};

#endif