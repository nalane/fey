#pragma once

/*
 * OpenGL implementation of model resource
 */

#include "model.hpp"

#include <map>

class glModel : public model {
private:
    GLuint vao;
    GLuint vbo;
    GLuint ubo;
    GLuint bindingPoint;

public:
    glModel(const std::string& name);
    ~glModel();

    virtual void bindData();
    virtual void draw(modelUniforms uniforms);
};