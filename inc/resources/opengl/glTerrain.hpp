#pragma once

/*
 * Terrain for opengl 
 */

#include "terrain.hpp"

class glTerrain : public terrain {
private:
    GLuint vao;
    GLuint vbo;
    GLuint ubo;
    GLuint bindingPoint;

public:
    glTerrain(const std::string& name);
    ~glTerrain();

    virtual void bindData();
    virtual void draw(modelUniforms uniforms);
};