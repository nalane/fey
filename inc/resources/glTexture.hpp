#pragma once

/*
 * OpenGL implementation of a texture
 */

#include "texture.hpp"
#include "glHeaders.hpp"

class glTexture : public texture {
private:
    GLuint texID;

public:
    glTexture(const std::string& name) : texture(name) { }
    ~glTexture();

    virtual bool loadTexture();
    void draw(GLuint index);
};