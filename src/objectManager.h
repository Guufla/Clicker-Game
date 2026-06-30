#ifndef OBJECT_MANAGER_H
#define OBJECT_MANAGER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <learnopengl/filesystem.h>
#include <iostream>

#include <iostream>
#include <algorithm> 
#include <vector>
#include <format>

#include "GameObject.h"

#include <soloud/soloud.h>
#include <soloud/soloud_wav.h>
#include <queue>



class ObjectManager
{
public:
    std::vector<GameObject*> Objects; // Make a list of all the possible bubble upgrades and their templates
    SpriteRenderer    *Renderer;
    
    // constructor(s)
    ObjectManager();

    void Update(float dt);
    
    void Render();
    
    void CreateObject(GameObject* object);
    
    void ObjectCollisions(float dt);
    
private:
    float CheckCollisions(GameObject &one, GameObject &two);
    
    void DestroyObjects();
    
    
    static bool SortXAxis(const GameObject* obj1, const GameObject* obj2);
    static bool SortYAxis(const GameObject* obj1, const GameObject* obj2);

    float Max(float a, float b);

    float Min(float a, float b);
};



#endif