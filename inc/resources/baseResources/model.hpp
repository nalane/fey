#pragma once

/*
 * Model resource base abstract class
 */

#include "vertexBasedResource.hpp"
#include "material.hpp"
#include "modelVertex.hpp"
#include "modelUniforms.hpp"
#include "texture.hpp"

#include <string>
#include <vector>

class model : public vertexBasedResource<modelVertex> {
protected:
    std::vector<material> materials;

public:
    model(const std::string& name) : vertexBasedResource(name) { }
    virtual ~model() { }

    static model* createModel(const std::string& name);

    void addMaterial(const material& mat);
    void setTexture(texture* tex);

    int getNumTextures() { return child_resources["textures"].size(); }
    std::vector<modelVertex> getVertices() const { return vertices; }
    std::vector<glm::vec3> getVertexPositions() const;
    material getMaterial() const { 
        // For now, shader only supports 1 material
        if (materials.size() > 0)
            return materials[0];
        return material(); 
    }

    virtual void draw(modelUniforms uniforms) = 0;
};