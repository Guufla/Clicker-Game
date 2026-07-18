#ifndef MENUMANAGER_H
#define MENUMANAGER_H

#include <RmlUi/Core.h>
#include <learnopengl/filesystem.h>


class MenuManager
{
public:
    


    // constructor(s)
    MenuManager();
    ~MenuManager();
    
    bool Initialize(Rml::Context* context);

    void Update(float dt);
    
    void Render(float dt);

    void ShowMenu();
    void HideMenu();
    void Shutdown();
    
    void ReloadMenu();
private:

    Rml::Context* context = nullptr;
    Rml::ElementDocument* menuDocument = nullptr;

};



#endif