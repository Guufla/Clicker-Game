#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "texture.h"
#include "spriteRenderer.h"


// Container object for holding all state relevant for a single
// game object entity. Each object in the game likely needs the
// minimal of state as described within GameObject.
class GameObject
{
public:
    // object state
    glm::vec2   Position, Size, Velocity;
    glm::vec3   Color;
    int         Tag; // 0 = default, 1 = bubble, 2 = click
    float       Rotation;
    bool        Destroyed;
    bool        IsDisabled;
    glm::vec3   ColliderShape; // (x,y,shape) shape:0=Disabled 1=circle 2=square 
    Texture2D   Sprite;	
    bool        CenterPivot; // If true, the position of the object will be the center of the object. If false, the position of the object will be the top left corner of the object.
    // constructor(s)
    GameObject();
    GameObject(
        glm::vec2 pos,
        glm::vec2 size,
        Texture2D sprite,
        bool      centerPivot,
        glm::vec3 colliderShape,
        glm::vec3 color,
        glm::vec2 velocity
    );

    GameObject(
        glm::vec2 pos,
        glm::vec2 size,
        Texture2D sprite,
        bool      centerPivot,
        glm::vec3 colliderShape,
        glm::vec2 velocity
    );

    GameObject(
        glm::vec2 pos,
        glm::vec2 size,
        Texture2D sprite,
        bool      centerPivot,
        glm::vec3 colliderShape,
        glm::vec3 color
    );

    GameObject(
        glm::vec2 pos,
        glm::vec2 size,
        Texture2D sprite,
        bool      centerPivot,
        glm::vec3 colliderShape
    );
    // GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f));
    // draw sprite
    virtual void Draw(SpriteRenderer &renderer);

    virtual void CollisionDetected(GameObject &other);
    virtual void Destroy()
    {
        
    }
    
    virtual ~GameObject() = default;
};

#endif