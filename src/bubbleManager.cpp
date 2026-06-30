#include "bubbleManager.h"

#include "game.h"
#include "objectManager.h"

extern Game BubbleBop;

extern ObjectManager* objectManager;

BubbleManager::BubbleManager()
{
    
}


void BubbleManager::Update(float dt)
{
    // Spawn bubbles randomly based on the spawn rate
    if(spawnTimer >= spawnRate)
    {
        glm::vec2 velocity = glm::vec2((rand() % 200) - 100, (rand() % 200) - 100) / 100.0f; // Random velocity between -1 and 1 on both axes
        
        SpawnBubble(glm::vec2(rand() % (BubbleBop.Width - 100) + 50.0f, rand() % (BubbleBop.Height - 100) + 50.0f), 50.0f,1.0f, 0.1f, velocity);
        spawnTimer = 0.0f;
    }
    else
    {
        spawnTimer += dt;
    }

    // Calls update function for each bubble object in the scene
    for(BubbleObject *bubble : Bubbles)
    {
        bubble->Update(dt);
    }
}

void BubbleManager::SpawnBubble(glm::vec2 pos,float radius,float points,float popTime,glm::vec2 movementDirection)
{
    // Creates bubble object and adds it to the bubble list
    BubbleObject *bubble = new BubbleObject(pos, radius, points, popTime, movementDirection, ResourceManager::GetTexture("bubble"),true, glm::vec3(radius, radius, 1.0f));
    bubble->Tag = 1; // When collisions occur this will help identify the object as a bubble
    
    Bubbles.push_back(bubble);
    objectManager->CreateObject(bubble);
}