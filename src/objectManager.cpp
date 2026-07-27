#include "objectManager.h"

#include "game.h"

extern Game BubbleBop;

ObjectManager::ObjectManager()
{
    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
}



void ObjectManager::Update(float dt)
{
    // The bubbles have to bounce off the borders of the menu
    
    // Update movement every frame based on the objects velocity variable
    for (GameObject *obj : this->Objects)
    {
        obj->Update(dt);
        
        obj->Position.x += obj->Velocity.x * dt * 100.0f;
        obj->Position.y += obj->Velocity.y * dt * 100.0f;
        
        if(obj->Tag == 1) // If the object is a bubble, make sure it does not go outside the screen bounds
        {
            if(obj->Position.x - obj->Size.x*0.5 < 0.0f)
            {
                obj->Position.x = obj->Size.x*0.5;
                obj->Velocity.x *= -1.0f;
            }
            else if(obj->Position.x + obj->Size.x*0.5 > BubbleBop.Width - BubbleBop.menuWidth)
            {
                obj->Position.x = BubbleBop.Width - BubbleBop.menuWidth - obj->Size.x*0.5;
                obj->Velocity.x *= -1.0f;
            }
            if(obj->Position.y - obj->Size.y*0.5 < 0.0f)
            {
                obj->Position.y = obj->Size.y*0.5;
                obj->Velocity.y *= -1.0f;
            }
            else if(obj->Position.y + obj->Size.y*0.5 > BubbleBop.Height)
            {
                obj->Position.y = BubbleBop.Height - obj->Size.y*0.5;
                obj->Velocity.y *= -1.0f;
            }
        }
    }
}

void ObjectManager::Render()
{
    for (GameObject *obj : Objects)
    {
        if(obj->IsDisabled == false)
        {
            obj->Draw(*Renderer);
        }
    }
}

void ObjectManager::CreateObject(GameObject* object)
{
    if (object != nullptr)
    {
        Objects.push_back(object);
    }
}


void ObjectManager::ObjectCollisions(float dt)
{
    // Do not check for collisions if there is only one game object in the scene
    if(this->Objects.size() <= 1)
    {
        return;
    }
    
    // Sort the game objects based on how far away they are from the left of the screen.
    std::sort(this->Objects.begin(), this->Objects.end(), ObjectManager::SortXAxis);
    
    // Loop through game object pairs
    for(int i = 0; i < this->Objects.size(); i++)
    {
        GameObject *obj1 = Objects[i];
        for(int j = i + 1; j < this->Objects.size(); j++)
        {
            GameObject *obj2 = Objects[j];
            
            // If game object 2 and game object 1 are too far apart do not run the check collisions function
            if(obj2->Position.x - obj2->Size.x > obj1->Position.x + obj1->Size.x)
            {
                break;
            }
            
            // When two objects are close enough run the collision function
            if(CheckCollisions(*obj1, *obj2))
            {
                // Make sure to fix walls constantly colliding with eachother
                obj1->CollisionDetected(*obj2);
                obj2->CollisionDetected(*obj1);
            }
        }
    }

    ObjectManager::DestroyObjects();
}

float ObjectManager::CheckCollisions(GameObject &one, GameObject &two)
{
    // Disabled Objects do not trigger collisions
    if(one.IsDisabled || two.IsDisabled)
    {
        return false;
    }
    // No collider on one of the objects
    if(one.ColliderShape.z == 0 || two.ColliderShape.z == 0)
    {
        return false;
    }
    // Circle-Circle Collision
    else if(one.ColliderShape.z == 1 && two.ColliderShape.z == 1)
    {
        float distance = sqrt(pow(one.Position.x - two.Position.x, 2) + pow(one.Position.y - two.Position.y, 2));
        return distance < one.ColliderShape.x + two.ColliderShape.x;
    }
    // Rectangle-Rectangle Collision
    else if(one.ColliderShape.z == 2 && two.ColliderShape.z == 2)
    {
        // collision x-axis?
        bool collisionX = one.Position.x + one.Size.x >= two.Position.x &&
        two.Position.x + two.Size.x >= one.Position.x;
        // collision y-axis?
        bool collisionY = one.Position.y + one.Size.y >= two.Position.y &&
        two.Position.y + two.Size.y >= one.Position.y;
        // collision only if on both axes
        return collisionX && collisionY;
    }
    else
    {
        // Circle-Square Collision
        GameObject &circle = one.ColliderShape.z == 1 ? one : two;
        GameObject &square = one.ColliderShape.z == 2 ? one : two;
        
        float closestX = ObjectManager::Max(square.Position.x, ObjectManager::Min(circle.Position.x, square.Position.x + square.Size.x));
        float closestY = ObjectManager::Max(square.Position.y, ObjectManager::Min(circle.Position.y, square.Position.y + square.Size.y));
        
        float distanceX = circle.Position.x - closestX;
        float distanceY = circle.Position.y - closestY;
        
        float distanceSquared = (distanceX * distanceX) + (distanceY * distanceY);
        
        return distanceSquared < (circle.ColliderShape.x * circle.ColliderShape.x);
    }
}

void ObjectManager::DestroyObjects()
{
    // Goes through the gameobjects and deletes any objects that are marked as Destroyed
    this->Objects.erase(
        std::remove_if(this->Objects.begin(), this->Objects.end(),
            [](GameObject* obj)
            {
                return obj->Destroyed;
            }),
        this->Objects.end()
    );
}



bool ObjectManager::SortXAxis(const GameObject* obj1, const GameObject* obj2)
{
    return obj1->Position.x < obj2->Position.x;
}

bool ObjectManager::SortYAxis(const GameObject* obj1, const GameObject* obj2)
{
    return obj1->Position.y < obj2->Position.y;
}

float ObjectManager::Max(float a, float b)
{
    return (a > b) ? a : b;
}

float ObjectManager::Min(float a, float b)
{
    return (a < b) ? a : b;
}


