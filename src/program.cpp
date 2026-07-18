#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "game.h"
#include "resourceManager.h"

#include <RmlUi/Core.h>
#include "RmlUi_Platform_GLFW.h"
#include "RmlUi_Renderer_GL3.h"

#include <iostream>

// GLFW function declarations
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void cursor_position_callback(GLFWwindow* window,double xpos,double ypos);


const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 600;

Game BubbleBop(SCR_WIDTH, SCR_HEIGHT);

double mouseX = 0.0;
double mouseY = 0.0;

bool mouseClicked = false;
bool mouseReleased = false;

int main(int arc, char* argv[])
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    
    
    GLFWwindow* window;
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Clicker Game", NULL, NULL);
    
    glfwMakeContextCurrent(window);
    
    
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    
    
    
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    
    
    // Setup Platform/Renderer backends
    // ImGui_ImplGlfw_InitForOpenGL(window, true);
    // ImGui_ImplOpenGL3_Init("#version 330");
    
    
    // OpenGL configuration
    // --------------------
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // initialize game
    // ---------------
    if (!BubbleBop.Init(window))
    {
        std::cerr << "Failed to initialize game.\n";

        glfwDestroyWindow(window);
        glfwTerminate();

        return -1;
    }

    // deltaTime variables
    // -------------------
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    while (!glfwWindowShouldClose(window))
    {
        // Calculate delta time of current frame
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        
        glfwPollEvents();

        // manage user input
        // -----------------
        BubbleBop.ProcessInput(deltaTime,mouseClicked);

        // update game state
        // -----------------
        BubbleBop.Update(deltaTime,mouseX,mouseY);

        // render
        // ----------------
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        BubbleBop.Render();
        
        
        
        // check collisions
        // ---------------
        BubbleBop.DoCollisions(deltaTime);

        glfwSwapBuffers(window);
    }

    BubbleBop.ShutdownRmlUi();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    // when a user presses the escape key, we set the WindowShouldClose property to true, closing the application
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            BubbleBop.Keys[key] = true;
        else if (action == GLFW_RELEASE)
        {
            BubbleBop.Keys[key] = false;
            BubbleBop.KeysProcessed[key] = false;
        }
    }
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    Rml::Context* context = BubbleBop.GetRmlContext();

    if (context != nullptr)
    {
        RmlGLFW::ProcessMouseButtonCallback(
            context,
            button,
            action,
            mods
        );
    }

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        //printf("Mouse Clicked\n");
        mouseClicked = true;
    }
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        //printf("Mouse Released\n");
        mouseClicked = false;
    }
}
\
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    BubbleBop.Resize(width, height);
}

void cursor_position_callback(GLFWwindow* window,double xpos,double ypos)
{
    mouseX = xpos;
    mouseY = ypos;
    
    Rml::Context* context = BubbleBop.GetRmlContext();

    if (context != nullptr)
    {
        RmlGLFW::ProcessCursorPosCallback(
            context,
            window,
            xpos,
            ypos,
            0
        );
    }
    //printf("Mouse Position: %f, %f\n",xpos,ypos);
}