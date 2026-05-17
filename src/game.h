#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>




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
    // constructor/destructor
    Game(unsigned int width, unsigned int height);
    ~Game();
    // initialize game state (load all shaders/textures/levels)
    void Init();
    // game loop
    void ProcessInput(float dt);
    void Update(float dt);
    void Render();
    void DoCollisions();

    // buy an item from the shop
    //void BuyItem(std::string item);

    // This will spawn a collider at the location which will either pop the bubbles nearby or buy an item
    void Click(float x, float y);

};

#endif