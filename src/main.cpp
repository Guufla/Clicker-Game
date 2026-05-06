#include <iostream>
#include <map>
#include <string>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader.h>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void RenderText(Shader &shader, std::string text, float x, float y, float scale, glm::vec3 color);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

unsigned int textVAO, textVBO;

// Game Variables
int money = 0;

// Input Variables
bool buttonPressed = false;


struct Character{
    unsigned int TextureID; // ID handle of the glyph texture
    glm::ivec2   Size;      // Size of glyph
    glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
    unsigned int Advance;   // Horizontal offset to advance to next glyph
};

std::map<char,Character> Characters;


float vertices[]={
    // positions         // colors
    0.0f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
    -0.5f,-0.5f, 0.0f,  0.0f, 0.0f, 1.0f
};

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

    /* OpenGL state */
    // glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    #pragma region Text Rendering
    /* compile and setup the shader */
    Shader shader(FileSystem::getPath("src/text.vs").c_str(), FileSystem::getPath("src/text.fs").c_str());
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), 0.0f, static_cast<float>(SCR_HEIGHT));
    shader.use();
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));


    /* Text Rendering */
    FT_Library ft;
    if(FT_Init_FreeType(&ft)) {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return -1;
    }

    FT_Face face;
    std::string font_name = FileSystem::getPath("fonts/Cardot-lxq6q.ttf");
    if (font_name.empty()) // Eventually switch the way you get this location so it is more dynamic
    {
        std::cout << "ERROR::FREETYPE: Failed to font_name" << std::endl;
        return -1;
    }

    if (FT_New_Face(ft, font_name.c_str(), 0, &face)) // Eventually switch the way you get this location so it is more dynamic
    {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        return -1;
    }
    else{
        // set size to load glyphs as
        FT_Set_Pixel_Sizes(face, 0, 48);  

        // disable byte-alignment restriction
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 

        // load first 128 characters of ASCII set
        /* In this loop we are taking each character of the 128 ascii set, retrieving their character glyphs, then for each character
        we generate a texture, set its options, store its metrics.*/
        for(unsigned char c = 0; c < 128; c++)
        {
            // Load the character glyph
            if(FT_Load_Char(face,c,FT_LOAD_RENDER))
            {
                std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                continue;
            }
            // generate texture
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
            );
            // set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            Character character = {
                texture, 
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                static_cast<unsigned int>(face->glyph->advance.x)
            };
            Characters.insert(std::pair<char, Character>(c, character));
        }
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    // destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    /* Configure textVAO and textVBO */
    
    glGenVertexArrays(1, &textVAO);
    glGenBuffers(1, &textVBO);
    glBindVertexArray(textVAO);
    glBindBuffer(GL_ARRAY_BUFFER, textVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    #pragma endregion Text Rendering

    /* Creating a triangle */

    Shader triangleShader(FileSystem::getPath("src/triangle.vs").c_str(), FileSystem::getPath("src/triangle.fs").c_str());

    unsigned int triangleVAO, triangleVBO;
    glGenVertexArrays(1, &triangleVAO);
    glGenBuffers(1, &triangleVBO);

    glBindVertexArray(triangleVAO);
    glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)0); // This is for layout (location = 0) in vec3 aPos;
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)(3 * sizeof(float))); // This is for layout (location = 1) in vec3 aColor;
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);


    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {

        processInput(window);

        /* Rendering Commands here*/
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        float timeValue = glfwGetTime();
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f;

        triangleShader.use();
        glBindVertexArray(triangleVAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawArrays(GL_TRIANGLES, 0, 3);


        RenderText(shader, "This is sample text", 23.0f, 23.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
        RenderText(shader, "Money Made: $ " + std::to_string(money), SCR_WIDTH / 6, SCR_HEIGHT - SCR_HEIGHT / 6, 1.0f, glm::vec3(0.3, 0.7f, 0.9f));

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
    // Keyboard input
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    

    // Mouse input
    int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    if (!buttonPressed && state == GLFW_PRESS)
    {
        money += 1;
        buttonPressed = true;
    }
    else if (state == GLFW_RELEASE)
    {
        buttonPressed = false;
    }
    
}

void RenderText(Shader &shader, std::string text, float x, float y, float scale, glm::vec3 color)
{
    float copyX = x; // This is used to reset the x position when we encounter a newline character

    // activate corresponding render state
    shader.use();
    glUniform3f(glGetUniformLocation(shader.ID, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(textVAO);


    // Interate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];
    
        if (*c == '\n') 
        {
            y -= 48.0f * 1.3 * scale; // 48 represents the line spacing when doing \n create a variable if you want to customize this
            x = copyX;
        }
        else if(*c == ' ')
        {
            x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
        }
        else{
            float xpos = x + ch.Bearing.x * scale;
            float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

            float w = ch.Size.x * scale;
            float h = ch.Size.y * scale;
            // update VBO for each character
            float vertices[6][4] = {
                { xpos,     ypos + h,   0.0f, 0.0f },            
                { xpos,     ypos,       0.0f, 1.0f },
                { xpos + w, ypos,       1.0f, 1.0f },

                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos + w, ypos,       1.0f, 1.0f },
                { xpos + w, ypos + h,   1.0f, 0.0f }           
            };
            // render glyph texture over quad
            glBindTexture(GL_TEXTURE_2D, ch.TextureID);
            // update content of VBO memory
            glBindBuffer(GL_ARRAY_BUFFER, textVBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            // render quad
            glDrawArrays(GL_TRIANGLES, 0, 6);
            // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
        }
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}