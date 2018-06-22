#pragma once

/*
 * Base abstract class for skybox resource
 */

#include "vertexBasedResource.hpp"
#include "skyboxVertex.hpp"
#include "skyboxUniforms.hpp"
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

class skybox : public vertexBasedResource<skyboxVertex> {
protected:
    const static std::vector<skyboxVertex> skyVertices;

public:
    skybox(const std::string& name) : vertexBasedResource(name) { vertices = skyVertices; }
    virtual ~skybox() { }

    static skybox* createSkybox(const std::string& name);

    virtual void setVertices(const std::vector<skyboxVertex>& vertices) { }
    virtual void setTextures(texture* tex, std::string texturePaths[NUM_SKYBOX_TEXTURES]) = 0;

    virtual void draw() = 0;
};