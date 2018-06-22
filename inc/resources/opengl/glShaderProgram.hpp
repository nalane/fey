#pragma once

/*
 * OpenGL implementation of a shader program
 */

#include "shaderProgram.hpp"
#include "glHeaders.hpp"

#include <map>

class glShaderProgram : public shaderProgram {
private:
    std::map<GLenum, GLuint> shaders;
    GLuint progID;

public:
    glShaderProgram(const std::string& name, const std::map<std::string, std::string>& shaderPaths) :
        shaderProgram(name, shaderPaths) { }
    ~glShaderProgram();

    virtual bool loadShaders(bool depthEnable = true, bool cullModeBackFaces = true);
    virtual void unloadShaders();

    void useProgram() { glUseProgram(progID); }
    GLuint getProgID() { return progID; }
};