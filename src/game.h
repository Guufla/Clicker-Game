#ifndef GAME_H
#define GAME_H



#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <learnopengl/filesystem.h>

#include "bubbleObject.h"
#include "resourceManager.h"
#include "spriteRenderer.h"
#include "clickObject.h"
#include "gameObject.h"
#include "audioManager.h"
#include "textRenderer.h"
#include "menuManager.h"
#include "bubbleManager.h"
#include "objectManager.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <RmlUi/Core.h>
#include <RmlUi/Debugger.h>
#include "RmlUi_Platform_GLFW.h"
#include "RmlUi_Renderer_GL3.h"

#include <iostream>
#include <algorithm> 
#include <vector>
#include <format>

#include <soloud/soloud.h>
#include <soloud/soloud_wav.h>


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
    float                   wallThickness;
    bool                    isMouseActive;
    bool                    isPaused;
    std::vector<BubbleObject*> Bubbles;
    // constructor/destructor
    Game(unsigned int width, unsigned int height);
    ~Game();
    // initialize game state (load all shaders/textures/levels)
    bool Init(GLFWwindow* glfwWindow);
    // game loop
    void ProcessInput(float dt,bool mouseClicked);
    void Update(float dt, double mouseX, double mouseY);
    void Render();
    void DoCollisions(float dt);
    
    
    void Resize(int width, int height);
    Rml::Context* GetRmlContext() const;
    void ShutdownRmlUi();
    
    
    void SpawnBubble(glm::vec2 pos,
                     float radius,
                     float points,
                     float popTime,
                     glm::vec2 movementDirection);

    bool CheckCollision(GameObject &one, GameObject &two);
    void DestroyObjects();
    bool IsDestroyed();
    static bool SortXAxis(const GameObject *obj1, const GameObject *obj2);
    static bool SortYAxis(const GameObject *obj1, const GameObject *obj2);


    void AddMoney(float amount);

    float Max(float a, float b);
    float Min(float a, float b);

    // buy an item from the shop
    //void BuyItem(std::string item);
    

    // Game managers
    AudioManager      *Audio;
    TextRenderer      *Text;
    BubbleManager     *bubbleManager;
    ObjectManager     *objectManager;
    MenuManager       *menuManager;


private:

    // Sprite renderer
    SpriteRenderer    *Renderer;
    
    // Persistent Game Objects
    ClickObject       *Click;
    // GameObject        *Wall1;
    // GameObject        *Wall2;
    // GameObject        *Wall3;
    // GameObject        *Wall4;
    


    GLFWwindow* window = nullptr;

    std::unique_ptr<SystemInterface_GLFW> rmlSystem;
    std::unique_ptr<RenderInterface_GL3> rmlRenderer;

    Rml::Context* rmlContext = nullptr;

    bool InitializeRmlUi();
};

#endif