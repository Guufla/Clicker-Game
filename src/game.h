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
    float                   clickTime;
    float                   clickRadius;
    float                   spawnRate;
    float                   spawnTimer;
    bool                    isMouseActive;
    std::vector<BubbleObject> Bubbles;
    std::vector<GameObject> GameObjects;
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

    bool CheckCollision(GameObject &one, GameObject &two);
    void DestroyObjects();
    bool IsDestroyed();
    static bool SortXAxis(const GameObject &obj1, const GameObject &obj2);
    static bool SortYAxis(const GameObject &obj1, const GameObject &obj2);

    // buy an item from the shop
    //void BuyItem(std::string item);

    

};

#endif