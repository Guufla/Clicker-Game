#ifndef BUBBLE_MANAGER_H
#define BUBBLE_MANAGER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <learnopengl/filesystem.h>
#include <iostream>
#include "bubbleObject.h"

#include <soloud/soloud.h>
#include <soloud/soloud_wav.h>
#include <queue>



class BubbleManager
{
public:
    std::vector<BubbleObject*> Bubbles;
    std::vector<GameObject> Upgrades; // Make a list of all the possible bubble upgrades and their templates
    float bubbleAdd;
    float bubbleMult;
    float spawnRate;
    float spawnTimer;
    
    
    // constructor(s)
    BubbleManager();
    BubbleManager(float bubbleAdd, float bubbleMult, float spawnRate);

    void Update(float dt);

    // Functions
    void SpawnBubble(glm::vec2 pos,
                    float radius,
                    float points,
                    float popTime,
                    glm::vec2 movementDirection);
    
private:
    
};



#endif