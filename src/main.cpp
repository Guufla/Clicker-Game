#include <ft2build.h>
#include FT_FREETYPE_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Clicker Game", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    

    /* Load OpenGL functions */
    if (!gladLoadGL())
    {
        glfwTerminate();
        return -1;
    }




    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {

        processInput(window);

        /* Rendering Commands here*/
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        FT_Library ft;
        if(FT_Init_FreeType(&ft)) {
            std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
            return -1;
        }

        FT_Face face;
        if (FT_New_Face(ft, "D:\\Repos\\Clicker-Game\\fonts\\Cardot-lxq6q.ttf", 0, &face))
        {
            std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;  
            return -1;
        }

        FT_Set_Pixel_Sizes(face, 0, 48); 

        if (FT_Load_Char(face, 'X', FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;  
            return -1;
        }

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}





void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}  

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}