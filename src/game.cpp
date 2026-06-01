

// Game-related State data
// All game objects and stuff in the scene will go here
#include <learnopengl/filesystem.h>


#include "game.h"
#include "resourceManager.h"
#include "spriteRenderer.h"
#include "bubbleObject.h"
#include "clickObject.h"
#include "gameObject.h"

#include <iostream>
#include <algorithm> 
#include <vector>


SpriteRenderer    *Renderer;
ClickObject       *Click;

Game::Game(unsigned int width, unsigned int height) 
    : Keys(), 
    KeysProcessed(), 
    Width(width), 
    Height(height), 
    money(0), 
    moneyLvl(0), 
    clickAdditive(1.0f), 
    clickMultiplier(1.0f), 
    clickTime(0.1f),
    clickRadius(0.2),
    spawnRate(3.0f),
    spawnTimer(3.0f),
    isMouseActive(false)
{ 

}


Game::~Game()
{
    delete Renderer;
}
// Initializes the game state (load all shaders/textures/levels)
void Game::Init()
{
    // load shaders
    ResourceManager::LoadShader(
        FileSystem::getPath("src/sprite.vs").c_str(),
        FileSystem::getPath("src/sprite.fs").c_str(),
        nullptr, "sprite");
    
    // configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width), static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("sprite", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    
    
    // load Textures
    ResourceManager::LoadTexture(FileSystem::getPath("resources/Images/backgroundImage.jpg").c_str(), false, "background");
    ResourceManager::LoadTexture(FileSystem::getPath("resources/Sprites/bubble.png").c_str(), true, "bubble");
    ResourceManager::LoadTexture(FileSystem::getPath("resources/Images/circle.png").c_str(), true, "circle");

    
    // set render-specific controls
    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
    

    // configure game objects
    Click = new ClickObject(glm::vec2(0, 0), 10.0f, 1.0f, ResourceManager::GetTexture("circle"));
    Click->IsDisabled = true;
    
    
    // audio
    
}


void Game::Update(float dt)
{
    // Spawn bubbles randomly based on the spawn rate
    if(spawnTimer >= spawnRate)
    {
        glm::vec2 velocity = glm::vec2((rand() % 3) - 1, (rand() % 3) - 1);
        while(velocity.x == 0 && velocity.y == 0)        {
            velocity = glm::vec2((rand() % 3) - 1, (rand() % 3) - 1);
        }
        
        //SpawnBubble(glm::vec2(rand() % (this->Width - 100) + 50.0f, rand() % (this->Height-100) + 50.0f), 50.0f, 1.0f, velocity);
        
        
        SpawnBubble(glm::vec2(rand() % this->Width, rand() % this->Height), 50.0f, 1.0f, glm::vec2(0.0f, 0.0f));
        //SpawnBubble(glm::vec2(rand() % this->Width, rand() % this->Height), 50.0f, 1.0f, glm::vec2(1.0f, 1.0f));
        spawnTimer = 0.0f;
    }
    else
    {
        spawnTimer += dt;
    }
    
    
    
    // Change movement direction based on movement direction
    for (BubbleObject &bubble : this->Bubbles)
    {
        bubble.Position.x += bubble.Velocity.x * dt * 100.0f;
        bubble.Position.y += bubble.Velocity.y * dt * 100.0f;
    }
    
    if(Click->IsDisabled == false)
    {
        Click->TimeAlive += dt;
        double xPos, yPos;
        glfwGetCursorPos(glfwGetCurrentContext(), &xPos, &yPos);
        Click->Move(static_cast<float>(xPos) - Click->Size.x / 2.0f,
                    static_cast<float>(yPos) - Click->Size.y / 2.0f);
        if(Click->TimeAlive >= clickTime)
        {
            Click->Disable();
        }
    }

}

void Game::ProcessInput(float dt, bool mouseClicked)
{
    if (isMouseActive == false && mouseClicked && Click->IsDisabled == true)
    {
        isMouseActive = true;
        double xPos, yPos;
        glfwGetCursorPos(glfwGetCurrentContext(), &xPos, &yPos);
        Click->Move(xPos, yPos);
        Click->TimeAlive = 0.0f;
        Click->Enable();
    }
    
    if(mouseClicked == false)
    {
        isMouseActive = false;
    }
}

void Game::Render()
{
    // Draw background
    Renderer->DrawSprite(
        ResourceManager::GetTexture("background"),
        glm::vec2(0.0f, 0.0f),
        glm::vec2(this->Width, this->Height),
        0.0f
    );

    // Draw GameObjects from bubble list
    for (GameObject &obj : this->GameObjects)
    {
        obj.Draw(*Renderer);
    }
    
    // Only render the click if it is enabled
    if(Click->IsDisabled == false)
    {
        //printf("Rendering Click Object\n");
        Click->Draw(*Renderer);
    }
    
    
}


void Game::SpawnBubble(glm::vec2 pos,float radius, float points, glm::vec2 movementDirection)
{
    // Creates bubble object and adds it to the bubble list
    BubbleObject bubble(pos, radius, points, movementDirection, ResourceManager::GetTexture("bubble"));
    this->GameObjects.push_back(bubble);
}


void Game::DoCollisions()
{
    // Bubble Object collisions X axis
    if(this->GameObjects.size() <= 1)
    {
        return;
    }
    
    std::sort(this->GameObjects.begin(), this->GameObjects.end(), Game::SortXAxis);
    
    for(int i = 0; i < this->GameObjects.size(); i++)
    {
        GameObject &obj1 = this->GameObjects[i];
        for(int j = i + 1; j < this->GameObjects.size(); j++)
        {
            GameObject &obj2 = this->GameObjects[j];
            if(obj2.Position.x - obj2.Size.x > obj1.Position.x + obj1.Size.x)
            {
                break;
            }
            if(CheckCollision(obj1, obj2))
            {
                // Handle collision
                //printf("Collision Detected\n");
                obj1.Destroyed = true;
                obj2.Destroyed = true;
            }
        }
    }

    Game::DestroyObjects();
}

bool Game::CheckCollision(GameObject &one, GameObject &two) 
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

void Game::DestroyObjects() 
{
    this->GameObjects.erase(std::remove_if(this->GameObjects.begin(), this->GameObjects.end(),
        [](GameObject &obj) { return obj.Destroyed; }), this->GameObjects.end());
}

bool Game::SortXAxis(const GameObject& obj1, const GameObject& obj2)
{
    return obj1.Position.x < obj2.Position.x;
}

bool Game::SortYAxis(const GameObject& obj1, const GameObject& obj2)
{
    return obj1.Position.y < obj2.Position.y;
}


// bool CheckCollision(GameObject &one, GameObject &two) 
// {
//     // collision x-axis?
//     bool collisionX = one.Position.x + one.Size.x >= two.Position.x &&
//         two.Position.x + two.Size.x >= one.Position.x;
//     // collision y-axis?
//     bool collisionY = one.Position.y + one.Size.y >= two.Position.y &&
//         two.Position.y + two.Size.y >= one.Position.y;
//     // collision only if on both axes
//     return collisionX && collisionY;
// }