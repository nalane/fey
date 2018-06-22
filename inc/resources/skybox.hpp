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
    const static std::vector<skyboxVertex> vertices;

public:
    skybox(const std::string& name) : raw_resource(name) { }
    virtual ~skybox() { }

    static skybox* createSkybox(const std::string& name);

    virtual void setTextures(texture* tex, std::string texturePaths[NUM_SKYBOX_TEXTURES]) = 0;
    void setShaderProgram(shaderProgram* shaderProg) {
        child_resources["shaderProgs"]["skybox"] = shaderProg;
    }

    virtual void bindData() = 0;
    virtual void draw() = 0;
};