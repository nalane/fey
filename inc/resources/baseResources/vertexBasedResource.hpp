#pragma once

/*
 * Base header for any object that is based on vertices for rendering
 */

#include "raw_resource.hpp"
#include "shaderProgram.hpp"

#include <vector>
#include <string>

template <typename vertStruct>
class vertexBasedResource : public raw_resource {
protected:
    std::vector<vertStruct> vertices;

public:
    vertexBasedResource(const std::string& name) : raw_resource(name) { }
    virtual ~vertexBasedResource() { }

    virtual void setVertices(const std::vector<vertStruct>& vertices) {
        this->vertices = vertices;
    }
    void setShaderProg(shaderProgram* prog) {
        child_resources["shaderProgs"]["default"] = prog;
    }

    virtual void bindData() = 0;
};