

#include "game.h"


Game::Game(unsigned int width, unsigned int height) 
    : Keys(), 
    KeysProcessed(), 
    Width(width),           // Width of the screen
    Height(height),         // Height of the screen
    money(0),               // Current money gained
    moneyLvl(0),            // Used to understand very large money amounts
    //clickAdditive(1.0f),    // Amount of extra money gained on click
    //clickMultiplier(1.0f),  // Multiplier on money gained on click
    clickTime(100.0f),
    clickRadius(10.0f),
    //spawnRate(0.01f),
    //spawnTimer(0.0f),
    wallThickness(40.0f),
    isMouseActive(false),
    isPaused(false)
{ 

}


Game::~Game()
{
    delete Renderer;
}

// Initializes the game state (load all shaders/textures/levels)

#pragma region Game Loop
bool Game::Init(GLFWwindow* glfwWindow)
{
    window = glfwWindow;

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

    ResourceManager::LoadTexture(FileSystem::getPath("resources/sprites/BlackPanel-1.png").c_str(), true, "panel1");
    ResourceManager::LoadTexture(FileSystem::getPath("resources/sprites/BlackPanel-2.png").c_str(), true, "panel2");
    ResourceManager::LoadTexture(FileSystem::getPath("resources/sprites/BlackPanel-3.png").c_str(), true, "panel3");
    
    // Eventually learn how to use this with a sprite sheet
    ResourceManager::LoadTexture(FileSystem::getPath("resources/sprites/BubbleFrame1.png").c_str(), true, "popFrame1");
    ResourceManager::LoadTexture(FileSystem::getPath("resources/sprites/BubbleFrame2.png").c_str(), true, "popFrame2");
    ResourceManager::LoadTexture(FileSystem::getPath("resources/sprites/BubbleFrame3.png").c_str(), true, "popFrame3");
    
    
    // set render-specific controls
    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
    
    // Audio
    Audio = new AudioManager(); // Initialize audio manager

    // Menus
    menuManager = new MenuManager(); // Initialize menus
    
    // Bubble manager 
    bubbleManager = new BubbleManager(0,0,0.01f,1000); // Initialize menus
    
    // Object Manager
    objectManager = new ObjectManager();
    
    // Upgrade Manager
    upgradeManager = new UpgradeManager();
    
    if (!InitializeRmlUi())
    {
        std::cerr << "Failed to initialize RmlUi.\n";
        return false;
    }
    if (!Rml::Debugger::Initialise(rmlContext))
    {
        std::cerr << "Failed to initialize RmlUi debugger.\n";
    }
    

    // configure game objects
    Click = new ClickObject(glm::vec2(0, 0), clickRadius, 1.0f, ResourceManager::GetTexture("circle"), glm::vec3(clickRadius, clickRadius, 1.0f));
    Click->IsDisabled = true;
    Click->Tag = 2; // When collisions occur this will help identify the object as the click object
    objectManager->CreateObject(Click);
    
    // // Walls used as the bounding boxes for the bubbles
    // Wall1 = new GameObject(
    //     glm::vec2(0.0f, -wallThickness),
    //     glm::vec2(static_cast<float>(this->Width), wallThickness),
    //     ResourceManager::GetTexture("blackSquare"),
    //     false,
    //     glm::vec3(static_cast<float>(this->Width), wallThickness, 2.0f),
    //     glm::vec3(1.0f),
    //     glm::vec2(0.0f)
    // );

    // // Walls used as the bounding boxes for the bubbles
    // Wall2 = new GameObject(
    //     glm::vec2(0.0f, static_cast<float>(this->Height)),
    //     glm::vec2(static_cast<float>(this->Width), wallThickness),
    //     ResourceManager::GetTexture("blackSquare"),
    //     false,
    //     glm::vec3(static_cast<float>(this->Width), wallThickness, 2.0f),
    //     glm::vec3(1.0f),
    //     glm::vec2(0.0f)
    // );

    // // Walls used as the bounding boxes for the bubbles
    // Wall3 = new GameObject(
    //     glm::vec2(-wallThickness, 0.0f),
    //     glm::vec2(wallThickness, static_cast<float>(this->Height)),
    //     ResourceManager::GetTexture("blackSquare"),
    //     false,
    //     glm::vec3(wallThickness, static_cast<float>(this->Height), 2.0f),
    //     glm::vec3(1.0f),
    //     glm::vec2(0.0f)
    // );

    // // Walls used as the bounding boxes for the bubbles
    // Wall4 = new GameObject(
    //     glm::vec2(static_cast<float>(this->Width), 0.0f),
    //     glm::vec2(wallThickness, static_cast<float>(this->Height)),
    //     ResourceManager::GetTexture("blackSquare"),
    //     false,
    //     glm::vec3(wallThickness, static_cast<float>(this->Height), 2.0f),
    //     glm::vec3(1.0f),
    //     glm::vec2(0.0f)
    // );
    
    // Add walls to the game objects vector
    // objectManager->CreateObject(Wall1);
    // objectManager->CreateObject(Wall2);
    // objectManager->CreateObject(Wall3);
    // objectManager->CreateObject(Wall4);

    // Text Renderer
    Text = new TextRenderer(static_cast<float>(this->Width),static_cast<float>(this->Height));
    std::string fontPath = FileSystem::getPath("resources/fonts/GAMERIA.ttf").c_str();
    Text->Load(fontPath,100);
    
    upgradeManager->Start();
    
    GetRmlMenuElement();
    return true;


}

void Game::Update(float dt,double mouseX,double mouseY)
{
    // Stops bubbles from moving and spawning when paused
    if(isPaused)
    {
        dt = 0.0f;
    }
    
    
    
    //std::cout << "Objects: " << GameObjects.size() << " dt: " << dt << std::endl;
    bubbleManager->Update(dt);
    
    objectManager->Update(dt);
    
    menuManager->Update(dt);
    
    if (rmlContext != nullptr)
    {
        rmlContext->Update();
    }
    
    // When the player clicks their mouse it will stay active for a few seconds
    if(Click->IsDisabled == false)
    {
        Click->TimeAlive += dt;
        Click->Move(static_cast<float>(mouseX) - Click->Size.x / 2.0f,
                    static_cast<float>(mouseY) - Click->Size.y / 2.0f);
        if(Click->TimeAlive >= clickTime)
        {
            Click->Disable();
        }
    }

    // If the screen is resized the menu dimensions need to be updated
    UpdateMenuDimensions();

    // Audio Clean Up
    Audio->CleanUpSounds();

}

void Game::ProcessInput(float dt, bool mouseClicked)
{
    // Processes initial mouse click
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

    // Pressing tab will pause the game and pressing it again will unpause
    if(Keys[GLFW_KEY_TAB] && !KeysProcessed[GLFW_KEY_TAB])
    {
        isPaused = !isPaused;
        KeysProcessed[GLFW_KEY_TAB] = true;
    }
    
    if (Keys[GLFW_KEY_F8] && !KeysProcessed[GLFW_KEY_F8])
    {
        Rml::Debugger::SetVisible(
            !Rml::Debugger::IsVisible()
        );

        KeysProcessed[GLFW_KEY_F8] = true;
    }
    
    if (Keys[GLFW_KEY_G] && !KeysProcessed[GLFW_KEY_G])
    {
        menuManager->ReloadMenu();

        KeysProcessed[GLFW_KEY_G] = false;
    }
    
    // Might be able to delete this will check soon
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
    objectManager->Render();

    // std::cout << std::to_string(money) << std::endl;
    
    Text->RenderValue(money,100,100,0.5f,glm::vec3(1.0f));
    
    // // Only render the click if it is enabled
    // if(Click->IsDisabled == false)
    // {
    //     //printf("Rendering Click Object\n");
    //     Click->Draw(*Renderer);
    // }
    
    // Used to get the corner of the bubble
    //bubbleManager->RenderDebugCenters(*Renderer);
    
    
    if (rmlContext != nullptr &&
        rmlRenderer != nullptr)
    {
        rmlRenderer->BeginFrame();
        rmlContext->Render();
        rmlRenderer->EndFrame();
    }
}

void Game::DoCollisions(float dt)
{
    // Skip collisions if the game is paused
    if(isPaused)
    {
        return;
    }
    
    objectManager->ObjectCollisions(dt);
    
}
#pragma endregion Game Loop

#pragma region Rml Functions
bool Game::InitializeRmlUi()
{
    if (window == nullptr)
        return false;

    int framebufferWidth = 0;
    int framebufferHeight = 0;

    glfwGetFramebufferSize(
        window,
        &framebufferWidth,
        &framebufferHeight
    );

    rmlSystem =
        std::make_unique<SystemInterface_GLFW>(window);

    rmlRenderer =
        std::make_unique<RenderInterface_GL3>();

    if (!(*rmlRenderer))
    {
        std::cerr << "Failed to create RmlUi renderer.\n";
        return false;
    }

    rmlRenderer->SetViewport(
        framebufferWidth,
        framebufferHeight
    );

    Rml::SetSystemInterface(rmlSystem.get());
    Rml::SetRenderInterface(rmlRenderer.get());

    if (!Rml::Initialise())
    {
        std::cerr << "Rml::Initialise failed.\n";
        return false;
    }

    rmlContext = Rml::CreateContext(
        "main",
        Rml::Vector2i(
            framebufferWidth,
            framebufferHeight
        )
    );

    if (rmlContext == nullptr)
    {
        std::cerr << "Failed to create RmlUi context.\n";
        return false;
    }

    if (!Rml::LoadFontFace(
            FileSystem::getPath("resources/fonts/Roboto-Regular.ttf").c_str()))
    {
        std::cerr << "Failed to load RmlUi font.\n";
    }
    if (!Rml::LoadFontFace(
            FileSystem::getPath("resources/fonts/GAMERIA.ttf").c_str()))
    {
        std::cerr << "Failed to load RmlUi font.\n";
    }

    if (!menuManager->Initialize(rmlContext))
    {
        std::cerr << "Failed to initialize menus.\n";
        return false;
    }

    return true;
}

void Game::Resize(int width, int height)
{
    Width = static_cast<unsigned int>(width);
    Height = static_cast<unsigned int>(height);

    if (rmlRenderer != nullptr)
    {
        rmlRenderer->SetViewport(width, height);
    }

    if (rmlContext != nullptr)
    {
        rmlContext->SetDimensions(
            Rml::Vector2i(width, height)
        );
    }
    
    glm::mat4 projection = glm::ortho(
        0.0f,
        static_cast<float>(Width),
        static_cast<float>(Height),
        0.0f,
        -1.0f,
        1.0f
    );

    ResourceManager::GetShader("sprite")
        .Use()
        .SetMatrix4("projection", projection);
        
    // Wall1->Position = glm::vec2(0.0f, -wallThickness);
    // Wall1->Size = glm::vec2(static_cast<float>(this->Width), wallThickness);
    // Wall1->ColliderShape = glm::vec3(static_cast<float>(this->Width), wallThickness, 2.0f);
        
        
}

void Game::ShutdownRmlUi()
{
    menuManager->Shutdown();

    if (rmlContext != nullptr)
    {
        Rml::RemoveContext(rmlContext->GetName());
        rmlContext = nullptr;
    }

    if (rmlRenderer != nullptr || rmlSystem != nullptr)
    {
        Rml::Shutdown();
    }

    rmlRenderer.reset();
    rmlSystem.reset();

    window = nullptr;
}

Rml::Context* Game::GetRmlContext() const
{
    return rmlContext;
}

void Game::GetRmlMenuElement()
{
    if (!rmlContext)
    {
        printf("Rml context is null\n");
        return;
    }

    rmlDocument = rmlContext->LoadDocument(FileSystem::getPath("resources/ui/main_menu.rml"));

    if (!rmlDocument)
    {
        printf("Failed to load main_menu.rml\n");
        return;
    }

    printf("Got document\n");

    rmlDocument->Show();

    menuElement = rmlDocument->GetElementById("Buy-Menu");

    if (menuElement)
    {
        printf("Got element\n");
    }
    else
    {
        printf("Could not find element Buy-Menu\n");
    }
}
#pragma endregion


#pragma region Helper Functions

void Game::AddMoney(float points)
{
    this->money += points;
    // Add click additive and multiplier logic either here or in the bubble object script
}

void Game::UpdateMenuDimensions()
{
    dimensions = menuElement->GetBox().GetSize(Rml::BoxArea::Border);
    
    menuWidth = dimensions.x;
    menuHeight = dimensions.y;
}
#pragma endregion Helper Functions