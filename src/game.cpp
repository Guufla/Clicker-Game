

#include "game.h"

SpriteRenderer    *Renderer;
ClickObject       *Click;
GameObject        *Wall1;
GameObject        *Wall2;
GameObject        *Wall3;
GameObject        *Wall4;
AudioManager      *Audio;



Game::Game(unsigned int width, unsigned int height) 
    : Keys(), 
    KeysProcessed(), 
    Width(width), 
    Height(height), 
    money(0), 
    moneyLvl(0), 
    clickAdditive(1.0f), 
    clickMultiplier(1.0f), 
    clickTime(100.0f),
    clickRadius(10.0f),
    spawnRate(0.1f),
    spawnTimer(0.1f),
    wallThickness(40.0f),
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
    ResourceManager::LoadTexture(FileSystem::getPath("resources/sprites/backgroundImage.jpg").c_str(), false, "background");
    ResourceManager::LoadTexture(FileSystem::getPath("resources/sprites/Bubble.png").c_str(), true, "bubble");
    ResourceManager::LoadTexture(FileSystem::getPath("resources/sprites/circle.png").c_str(), true, "circle");
    ResourceManager::LoadTexture(FileSystem::getPath("resources/sprites/black.png").c_str(), true, "blackSquare");
    
    // Eventually learn how to use this with a sprite sheet
    ResourceManager::LoadTexture(FileSystem::getPath("resources/sprites/BubbleFrame1.png").c_str(), true, "popFrame1");
    ResourceManager::LoadTexture(FileSystem::getPath("resources/sprites/BubbleFrame2.png").c_str(), true, "popFrame2");
    ResourceManager::LoadTexture(FileSystem::getPath("resources/sprites/BubbleFrame3.png").c_str(), true, "popFrame3");

    
    // set render-specific controls
    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
    

    // configure game objects
    Click = new ClickObject(glm::vec2(0, 0), clickRadius, 1.0f, ResourceManager::GetTexture("circle"), glm::vec3(clickRadius, clickRadius, 1.0f));
    Click->IsDisabled = true;
    Click->Tag = 2; // When collisions occur this will help identify the object as the click object
    this->GameObjects.push_back(Click);
    
    
    Wall1 = new GameObject(
    glm::vec2(0.0f, -wallThickness),
    glm::vec2(static_cast<float>(this->Width), wallThickness),
    ResourceManager::GetTexture("blackSquare"),
    false,
    glm::vec3(static_cast<float>(this->Width), wallThickness, 2.0f),
    glm::vec3(1.0f),
    glm::vec2(0.0f)
    );

    Wall2 = new GameObject(
        glm::vec2(0.0f, static_cast<float>(this->Height)),
        glm::vec2(static_cast<float>(this->Width), wallThickness),
        ResourceManager::GetTexture("blackSquare"),
        false,
        glm::vec3(static_cast<float>(this->Width), wallThickness, 2.0f),
        glm::vec3(1.0f),
        glm::vec2(0.0f)
    );

    Wall3 = new GameObject(
        glm::vec2(-wallThickness, 0.0f),
        glm::vec2(wallThickness, static_cast<float>(this->Height)),
        ResourceManager::GetTexture("blackSquare"),
        false,
        glm::vec3(wallThickness, static_cast<float>(this->Height), 2.0f),
        glm::vec3(1.0f),
        glm::vec2(0.0f)
    );

    Wall4 = new GameObject(
        glm::vec2(static_cast<float>(this->Width), 0.0f),
        glm::vec2(wallThickness, static_cast<float>(this->Height)),
        ResourceManager::GetTexture("blackSquare"),
        false,
        glm::vec3(wallThickness, static_cast<float>(this->Height), 2.0f),
        glm::vec3(1.0f),
        glm::vec2(0.0f)
    );
    
    this->GameObjects.push_back(Wall1);
    this->GameObjects.push_back(Wall2);
    this->GameObjects.push_back(Wall3);
    this->GameObjects.push_back(Wall4);


    // audio
    Audio = new AudioManager();

}


void Game::Update(float dt)
{
    
    // Spawn bubbles randomly based on the spawn rate
    if(spawnTimer >= spawnRate)
    {
        glm::vec2 velocity = glm::vec2((rand() % 200) - 100, (rand() % 200) - 100) / 100.0f; // Random velocity between -1 and 1 on both axes
        
        SpawnBubble(glm::vec2(rand() % (this->Width - 100) + 50.0f, rand() % (this->Height - 100) + 50.0f), 50.0f,1.0f, 0.1f, velocity);
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
    
    //std::cout << "Objects: " << GameObjects.size() << " dt: " << dt << std::endl;
    
    
    // // Change movement direction based on movement directions
    for (GameObject *obj : this->GameObjects)
    {
        obj->Position.x += obj->Velocity.x * dt * 100.0f;
        obj->Position.y += obj->Velocity.y * dt * 100.0f;
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
        Click->Move(
            static_cast<float>(xPos) - Click->Size.x / 2.0f,
            static_cast<float>(yPos) - Click->Size.y / 2.0f
        );
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

    // Draw GameObjects from gameObject list
    for (GameObject *obj : GameObjects)
    {
        if(obj->IsDisabled == false)
        {
            obj->Draw(*Renderer);
        }
    }
    
    // // Only render the click if it is enabled
    // if(Click->IsDisabled == false)
    // {
    //     //printf("Rendering Click Object\n");
    //     Click->Draw(*Renderer);
    // }
    
    
}


void Game::SpawnBubble(glm::vec2 pos,float radius, float points,float popTime, glm::vec2 movementDirection)
{
    // Creates bubble object and adds it to the bubble list
    BubbleObject *bubble = new BubbleObject(pos, radius, points, popTime, movementDirection, ResourceManager::GetTexture("bubble"),true, glm::vec3(radius, radius, 1.0f));
    bubble->Tag = 1; // When collisions occur this will help identify the object as a bubble
    
    Bubbles.push_back(bubble);
    GameObjects.push_back(bubble);
}


void Game::DoCollisions(float dt)
{
    // Do not check for collisions if there is only one game object in the scene
    if(this->GameObjects.size() <= 1)
    {
        return;
    }
    
    // Sort the game objects based on how far away they are from the left of the screen.
    std::sort(this->GameObjects.begin(), this->GameObjects.end(), Game::SortXAxis);
    
    // Loop through game object pairs
    for(int i = 0; i < this->GameObjects.size(); i++)
    {
        GameObject *obj1 = GameObjects[i];
        for(int j = i + 1; j < this->GameObjects.size(); j++)
        {
            GameObject *obj2 = GameObjects[j];
            
            // If game object 2 and game object 1 are too far apart do not run the check collisions function
            if(obj2->Position.x - obj2->Size.x > obj1->Position.x + obj1->Size.x)
            {
                break;
            }
            
            // When two objects are close enough run the collision function
            if(CheckCollision(*obj1, *obj2))
            {
                // Make sure to fix walls constantly colliding with eachother
                obj1->CollisionDetected(*obj2);
                obj2->CollisionDetected(*obj1);
            }
        }
    }

    Game::DestroyObjects();
}

bool Game::CheckCollision(GameObject &one, GameObject &two) 
{
    // Disabled Objects do not trigger collisions
    if(one.IsDisabled || two.IsDisabled)
    {
        return false;
    }
    // No collider on one of the objects
    if(one.ColliderShape.z == 0 || two.ColliderShape.z == 0)
    {
        return false;
    }
    // Circle-Circle Collision
    else if(one.ColliderShape.z == 1 && two.ColliderShape.z == 1)
    {
        float distance = sqrt(pow(one.Position.x - two.Position.x, 2) + pow(one.Position.y - two.Position.y, 2));
        return distance < one.ColliderShape.x + two.ColliderShape.x;
    }
    // Rectangle-Rectangle Collision
    else if(one.ColliderShape.z == 2 && two.ColliderShape.z == 2)
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
    else
    {
        // Circle-Square Collision
        GameObject &circle = one.ColliderShape.z == 1 ? one : two;
        GameObject &square = one.ColliderShape.z == 2 ? one : two;
        
        float closestX = Game::Max(square.Position.x, Game::Min(circle.Position.x, square.Position.x + square.Size.x));
        float closestY = Game::Max(square.Position.y, Game::Min(circle.Position.y, square.Position.y + square.Size.y));
        
        float distanceX = circle.Position.x - closestX;
        float distanceY = circle.Position.y - closestY;
        
        float distanceSquared = (distanceX * distanceX) + (distanceY * distanceY);
        
        return distanceSquared < (circle.ColliderShape.x * circle.ColliderShape.x);
    }
}

void Game::DestroyObjects() 
{
    this->GameObjects.erase(
    std::remove_if(this->GameObjects.begin(), this->GameObjects.end(),
        [](GameObject* obj)
        {
            return obj->Destroyed;
        }),
    this->GameObjects.end()
);
}

bool Game::SortXAxis(const GameObject* obj1, const GameObject* obj2)
{
    return obj1->Position.x < obj2->Position.x;
}

bool Game::SortYAxis(const GameObject* obj1, const GameObject* obj2)
{
    return obj1->Position.y < obj2->Position.y;
}

float Game::Max(float a, float b)
{
    return (a > b) ? a : b;
}

float Game::Min(float a, float b)
{
    return (a < b) ? a : b;
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