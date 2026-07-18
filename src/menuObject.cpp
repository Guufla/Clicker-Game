
#include "menuObject.h"
#include "game.h" // Allows us to call the game manager


extern Game BubbleBop;

MenuObject::MenuObject() 
    : GameObject(),pos(glm::vec2(0,0)),Size(0){ }

void MenuObject::CollisionDetected(GameObject &other)
{
    
}