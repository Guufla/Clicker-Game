#include "bubbleManager.h"

#include "game.h"
#include "objectManager.h"

extern Game BubbleBop;

BubbleManager::BubbleManager()
{
    
}


void BubbleManager::Update(float dt)
{
    // Spawn bubbles randomly based on the spawn rate
    if(spawnTimer >= spawnRate)
    {
        glm::vec2 velocity = glm::vec2((rand() % 200) - 100, (rand() % 200) - 100) / 100.0f; // Random velocity between -1 and 1 on both axes
        
        // Spawns bubbles Remember to re-enable this
        SpawnBubble(glm::vec2(rand() % (BubbleBop.Width - 100) + 50.0f, rand() % (BubbleBop.Height - 100) + 50.0f), 30.0f,1.0f, 0.1f, velocity);
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
    BubbleBop.objectManager->CreateObject(bubble);
}


// Debug statement 
void BubbleManager::RenderDebugCenters(
    SpriteRenderer& renderer
)
{
    const float markerSize = 8.0f;

    for (BubbleObject* bubble : Bubbles)
    {
        if (bubble == nullptr)
            continue;

        glm::vec2 center =
            bubble->Position +
            bubble->Size * 0.5f;

        // DrawSprite positions sprites using their top-left corner,
        // so subtract half the marker size to center it.
        glm::vec2 markerPosition =
            center -
            glm::vec2(markerSize * 0.5f);

        renderer.DrawSprite(
            ResourceManager::GetTexture("circle"),
            markerPosition,
            glm::vec2(markerSize, markerSize),
            0.0f,
            glm::vec3(1.0f, 0.0f, 0.0f)
        );
    }
}