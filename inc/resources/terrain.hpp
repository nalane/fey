#pragma once

/*
 * Generic terrain base class
 */

#include "raw_resource.hpp"
#include "glHeaders.hpp"
#include "texture.hpp"
#include "shaderProgram.hpp"
#include "modelUniforms.hpp"
#include "terrainVertex.hpp"
#include "material.hpp"

#include <vector>
#include <string>

class terrain : public raw_resource {
protected:
    // Control points will be stored in row major order
    std::vector<terrainVertex> controlPoints;
    material mat;

public:
    const static int PATCH_SIZE = 16;

    terrain(const std::string& name) : raw_resource(name) { }
    virtual ~terrain() { }

    static terrain* createTerrain(const std::string& name);
    void setControlPoints(const std::vector<terrainVertex>& controlPoints) {
        this->controlPoints = controlPoints;
    }

    void setTexture(texture* tex) {
        child_resources["textures"]["terrain"] = tex;
    }
    void setShaderProg(shaderProgram* prog) {
        child_resources["shaderProgs"]["terrain"] = prog;
    }

    virtual void bindData() = 0;
    virtual void draw(modelUniforms uniforms) = 0;

    material getMaterial() const { return mat; }
};