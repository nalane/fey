#pragma once

/*
 * OpenGL implementation of skybox
 */

#include "skybox.hpp"

class glSkybox : public skybox {
private:
    GLuint texID;
    GLuint vao;
    GLuint vbo;
    GLuint ubo;

public:
    glSkybox(const std::string& name);
    ~glSkybox();

    virtual void setTextures(texture* tex, std::string texturePaths[NUM_SKYBOX_TEXTURES]);
    virtual void bindData();
    virtual void draw();
};