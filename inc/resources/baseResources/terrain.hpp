#pragma once

/*
 * Generic terrain base class
 */

#include "vertexBasedResource.hpp"
#include "texture.hpp"
#include "terrainVertex.hpp"
#include "modelUniforms.hpp"
#include "material.hpp"

#include <vector>
#include <string>

class terrain : public vertexBasedResource<terrainVertex> {
protected:
    material mat;

public:
    const static int PATCH_SIZE = 16;

    terrain(const std::string& name) : vertexBasedResource(name) { }
    virtual ~terrain() { }

    static terrain* createTerrain(const std::string& name);

    void setTexture(texture* tex) {
        child_resources["textures"]["terrain"] = tex;
    }

    material getMaterial() const { return mat; }
    virtual void draw(modelUniforms uniforms) = 0;
};