#ifndef MENUMANAGER_H
#define MENUMANAGER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "texture.h"
#include "spriteRenderer.h"
#include "gameObject.h"
#include "resourceManager.h"
#include "audioManager.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


#include <iostream>


class MenuManager
{
public:
    


    // constructor(s)
    MenuManager();

    void Update(float dt);

    void OpenMainMenu();

    void CloseMainMenu();

    void InitializeBuyMenu();

    void HideBuyMenu(); // This will be later on when i learn how to properly resize the window

    // Functions
private:

    bool isOpen = true;
    float panelWidth = 300.0f;

};



#endif