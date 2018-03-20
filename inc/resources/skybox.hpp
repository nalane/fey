#pragma once

/*
 * Base abstract class for skybox resource
 */

#include "raw_resource.hpp"
#include "skyboxVertex.hpp"
#include "shaderProgram.hpp"
#include "texture.hpp"

#include <string>
#include <vector>

#define SKYBOX_RIGHT        0
#define SKYBOX_LEFT         1
#define SKYBOX_TOP          2
#define SKYBOX_BOTTOM       3
#define SKYBOX_BACK         4
#define SKYBOX_FRONT        5
#define NUM_SKYBOX_TEXTURES 6

class skybox : public raw_resource {
protected:
    std::vector<skyboxVertex> vertices;

public:
    skybox(const std::string& name) : raw_resource(name) { }
    virtual ~skybox() { }

    static skybox* createSkybox(const std::string& name);

    void setShaderProgram(shaderProgram* shaderProg) {
        child_resources["shaderProgs"]["skybox"] = shaderProg;
    }
    void setTextures(texture* tex) {
        child_resources["textures"]["skybox"] = tex;
    }

    virtual void bindVertices() = 0;
    virtual void bindDescriptors() = 0;

    virtual void draw() = 0;
};