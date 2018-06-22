#pragma once

/*
 * Model resource base abstract class
 */

#include "glHeaders.hpp"
#include "raw_resource.hpp"
#include "material.hpp"
#include "modelVertex.hpp"
#include "texture.hpp"
#include "shaderProgram.hpp"
#include "modelUniforms.hpp"

#include <string>
#include <vector>

class model : public raw_resource {
protected:
    std::vector<material> materials;
    std::vector<modelVertex> vertices;

public:
    model(const std::string& name) : raw_resource(name) { }
    virtual ~model() { }

    static model* createModel(const std::string& name);

    void addMaterial(const material& mat);
    void setTexture(texture* tex);
    void setVertices(const std::vector<glm::vec3>& vertexList);
    void setUVMapping(const std::vector<glm::vec2>& uvList);
    void setNormals(const std::vector<glm::vec3>& normalList);
    void setShaderProgram(shaderProgram* shaderProg) {
        child_resources["shaderProgs"]["default"] = shaderProg;
    }

    virtual void bindData() = 0;
    virtual void draw(modelUniforms uniforms) = 0;

    int getNumTextures() { return child_resources["textures"].size(); }
    std::vector<modelVertex> getVertices() const { return vertices; }
    std::vector<glm::vec3> getVertexPositions() const;
    material getMaterial() const { 
        // For now, shader only supports 1 material
        if (materials.size() > 0)
            return materials[0];
        return material(); 
    }
};