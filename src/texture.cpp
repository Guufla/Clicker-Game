#include <glad/glad.h>

#include "texture.h"

// holds the ID of the texture object, used for all texture operations to reference to this particular texture
unsigned int ID;
// texture image dimensions
unsigned int width, height; // width and height of loaded image in pixels
// texture Format
unsigned int internalFormat; // format of texture object
unsigned int imageFormat; // format of loaded image
// texture configuration
unsigned int wrapS; // wrapping mode on S axis
unsigned int wrapT; // wrapping mode on T axis
unsigned int filterMin; // filtering mode if texture pixels < screen pixels
unsigned int filterMax; // filtering mode if texture pixels > screen pixels
// constructor (sets default texture modes)
Texture2D::Texture2D()
    : width(0), height(0), internalFormat(GL_RGB), imageFormat(GL_RGB), wrapS(GL_REPEAT), wrapT(GL_REPEAT), filterMin(GL_LINEAR), filterMax(GL_LINEAR)
{
    glGenTextures(1, &this->ID);
}
// generates texture from image data
void Texture2D :: Generate(unsigned int width, unsigned int height, unsigned char* data)
{
    this->width = width;
    this->height = height;
    
    glBindTexture(GL_TEXTURE_2D, this->ID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->wrapT);
    
    glTexImage2D(GL_TEXTURE_2D, 0, this->internalFormat, width, height, 0, this->imageFormat, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->filterMin);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->filterMax);
    
    // unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);


};
// binds the texture as the current active GL_TEXTURE_2D texture object
void Texture2D::Bind() const
{
    glBindTexture(GL_TEXTURE_2D, this->ID);
};