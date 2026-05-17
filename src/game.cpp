

// Game-related State data
// All game objects and stuff in the scene will go here
#include <learnopengl/filesystem.h>


#include "game.h"
#include "resourceManager.h"
#include "spriteRenderer.h"
#include <iostream>


SpriteRenderer    *Renderer;

Game::Game(unsigned int width, unsigned int height) 
    : Keys(), KeysProcessed(), Width(width), Height(height), money(0), moneyLvl(0), clickAdditive(1.0f), clickMultiplier(1.0f), moustRadius(0.2)
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
    ResourceManager::LoadTexture(FileSystem::getPath("resources/Images/backgroundImage.jpg").c_str(), true, "background");

    
    // set render-specific controls
    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
    
    // configure game objects
    
    // audio
}


void Game::Update(float dt)
{

}

void Game::ProcessInput(float dt)
{

}

void Game::Render()
{

    Renderer->DrawSprite(ResourceManager::GetTexture("background"),  glm::vec2(0.0f, 0.0f),glm::vec2(this->Width,this->Height) , 0.0f);
}

void Game::DoCollisions()
{

}