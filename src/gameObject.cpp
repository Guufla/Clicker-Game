

#include "gameObject.h"


GameObject::GameObject() 
    : Position(0.0f, 0.0f),
      Size(1.0f, 1.0f),
      Velocity(0.0f),
      Color(1.0f),
      Rotation(0.0f),
      Sprite(),
      CenterPivot(false),
      Destroyed(false),
      ColliderShape(20.0f, 20.0f, 2.0f)
{ }

GameObject::GameObject(
    glm::vec2 pos,
    glm::vec2 size,
    Texture2D sprite,
    bool      centerPivot,
    glm::vec3 colliderShape,
    glm::vec3 color,
    glm::vec2 velocity
) 
    : Position(pos),
      Size(size),
      Velocity(velocity),
      Color(color),
      Rotation(0.0f),
      Sprite(sprite),
      CenterPivot(centerPivot),
      ColliderShape(colliderShape),
      Destroyed(false),
      IsDisabled(false)
{ }

GameObject::GameObject(
    glm::vec2 pos,
    glm::vec2 size,
    Texture2D sprite,
    bool      centerPivot,
    glm::vec3 colliderShape,
    glm::vec2 velocity
) 
    : Position(pos),
      Size(size),
      Velocity(velocity),
      Color(glm::vec3(1.0f)),
      Rotation(0.0f),
      Sprite(sprite),
      CenterPivot(centerPivot),
      ColliderShape(colliderShape),
      Destroyed(false),
      IsDisabled(false)
{ }

GameObject::GameObject(
    glm::vec2 pos,
    glm::vec2 size,
    Texture2D sprite,
    bool      centerPivot,
    glm::vec3 colliderShape,
    glm::vec3 color
) 
    : Position(pos),
      Size(size),
      Velocity(glm::vec2(0.0f)),
      Color(color),
      Rotation(0.0f),
      Sprite(sprite),
      CenterPivot(centerPivot),
      ColliderShape(colliderShape),
      Destroyed(false),
      IsDisabled(false)
{ }

GameObject::GameObject(
    glm::vec2 pos,
    glm::vec2 size,
    Texture2D sprite,
    bool      centerPivot,
    glm::vec3 colliderShape
) 
    : Position(pos),
      Size(size),
      Velocity(glm::vec2(0.0f)),
      Color(glm::vec3(1.0f)),
      Rotation(0.0f),
      Sprite(sprite),
      CenterPivot(centerPivot),
      ColliderShape(colliderShape),
      Destroyed(false),
      IsDisabled(false)
{ }
void GameObject::Draw(SpriteRenderer &renderer)
{
    renderer.DrawSprite(this->Sprite, this->Position, this->Size, this->Rotation, this->Color, this->CenterPivot);
}

void GameObject::CollisionDetected(GameObject &other)
{
    
}