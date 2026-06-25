
#include "menuObject.h"
#include "game.h" // Allows us to call the game manager


extern AudioManager *Audio;
extern Game BubbleBop;

MenuObject::MenuObject() 
    : GameObject(), Points(1.0f) ,Radius(1.0f),PopTime(2.0f),PopTimer(0.0f),IsPopping(false){ }

MenuObject::MenuObject(glm::vec2 pos, float radius, float points, float popTime, glm::vec2 velocity, Texture2D sprite, bool centerPivot, glm::vec3 colliderShape)
{
}

void MenuObject::CollisionDetected(GameObject &other)
{
    
}