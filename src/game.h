#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include "bubbleObject.h"

class Game
{
public:
    // game state
    bool                    Keys[1024];
    bool                    KeysProcessed[1024];
    unsigned int            Width, Height;
    float                   money;
    int                     moneyLvl;
    float                   clickAdditive;
    float                   clickMultiplier;
    float                   moustRadius;
    float                   spawnRate;
    float                   spawnTimer;
    std::vector<BubbleObject> Bubbles;
    // constructor/destructor
    Game(unsigned int width, unsigned int height);
    ~Game();
    // initialize game state (load all shaders/textures/levels)
    void Init();
    // game loop
    void ProcessInput(float dt,bool mouseClicked);
    void Update(float dt);
    void Render();
    void DoCollisions();
    void SpawnBubble(glm::vec2 pos,
        float radius, 
        float points, 
        glm::vec2 movementDirection);

    // buy an item from the shop
    //void BuyItem(std::string item);

    

};

#endif