#include "bubbleManager.h"

#include "game.h"
#include "objectManager.h"

extern Game BubbleBop;

BubbleManager::BubbleManager()
{
    
}

BubbleManager::BubbleManager(
    float additive, 
    float multiplier, 
    float spawnRateValue,
    float spawnMaxValue
)
    :   bubbleAdd(additive),
        bubbleMult(multiplier),
        spawnRate(spawnRateValue),
        spawnMax(spawnMaxValue),
        spawnTimer(0.0f),
        bubbleCount(0)
{
}


void BubbleManager::Update(float dt)
{
    // Spawn bubbles randomly based on the spawn rate
    if(spawnTimer >= spawnRate && bubbleCount < 1000)
    {
        glm::vec2 velocity = glm::vec2((rand() % 200) - 100, (rand() % 200) - 100) / 100.0f; // Random velocity between -1 and 1 on both axes
        
        // Spawns bubbles Remember to re-enable this
        //SpawnBubble(glm::vec2(rand() % (BubbleBop.Width - (int)BubbleBop.menuWidth - 100) + 50.0f, rand() % (BubbleBop.Height - 100) + 50.0f), 30.0f,1.0f, 0.1f, velocity);
        spawnTimer = 0.0f;
        bubbleCount++;
    }
    else
    {
        spawnTimer += dt;
    }
    
    
    //std::cout << bubbleCount << std::endl;

    
}

void BubbleManager::SpawnBubble(glm::vec2 pos,float radius,float points,float popTime,glm::vec2 movementDirection)
{
    // Creates bubble object and adds it to the bubble list
    BubbleObject *bubble = new BubbleObject(pos, radius, points, popTime, movementDirection, ResourceManager::GetTexture("bubble"),true, glm::vec3(radius, radius, 1.0f));
    bubble->Tag = 1; // When collisions occur this will help identify the object as a bubble
    
    
    BubbleBop.objectManager->CreateObject(bubble);
}

void BubbleManager::DeleteBubble()
{
    bubbleCount--;

}


