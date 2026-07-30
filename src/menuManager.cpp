#include "menuManager.h"

#include <iostream>

#include "game.h"

extern Game BubbleBop;

MenuManager::MenuManager()
{

}
MenuManager::~MenuManager()
{
    Shutdown();
}

bool MenuManager::Initialize(Rml::Context* newContext)
{
    if (newContext == nullptr)
    {
        std::cerr << "MenuManager received a null RmlUi context.\n";
        return false;
    }

    context = newContext;
    
    BubbleBop.upgradeManager->InitializeDataModel(context);
    BubbleBop.upgradeManager->LoadGeneralUpgrades();
    

    const Rml::String menuPath = FileSystem::getPath("resources/ui/main_menu.rml").c_str();

    std::cout << "Loading RmlUi menu: "
              << menuPath << '\n';

    menuDocument = context->LoadDocument(menuPath);

    if (menuDocument == nullptr)
    {
        std::cerr
            << "Failed to load RmlUi document: "
            << menuPath << '\n';

        context = nullptr;
        return false;
    }

    menuDocument->Show();

    std::cout << "RmlUi menu loaded successfully.\n";
    return true;
}

void MenuManager::Update(float dt)
{
    // Menu-specific value updates go here.
}

void MenuManager::ShowMenu()
{
    if (menuDocument != nullptr)
        menuDocument->Show();
}

void MenuManager::HideMenu()
{
    if (menuDocument != nullptr)
        menuDocument->Hide();
}

void MenuManager::Shutdown()
{
    if (menuDocument != nullptr)
    {
        menuDocument->Close();
        menuDocument = nullptr;
    }

    context = nullptr;
}


// Debug to reload menu for rcss and rml changes
void MenuManager::ReloadMenu()
{
    if (context == nullptr)
        {
            std::cerr << "Cannot reload menu: context is null.\n";
            return;
        }

        // Close the old document first.
        if (menuDocument != nullptr)
        {
            menuDocument->Close();
            menuDocument = nullptr;
        }

        // Force RmlUi to read the changed RML and RCSS files again.
        Rml::Factory::ClearStyleSheetCache();
        Rml::Factory::ClearTemplateCache();

        const std::string menuPath =
            FileSystem::getPath("resources/ui/main_menu.rml");

        menuDocument = context->LoadDocument(menuPath);

        if (menuDocument == nullptr)
        {
            std::cerr
                << "Failed to reload menu: "
                << menuPath
                << '\n';

            return;
        }

        menuDocument->Show();

        std::cout << "RML and RCSS reloaded.\n";
}