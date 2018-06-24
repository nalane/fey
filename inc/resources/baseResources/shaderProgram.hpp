#pragma once

/*
 * Base abstract classs for shader program
 */

#include "raw_resource.hpp"

#include <map>
#include <string>
#include <map>

class shaderProgram : public raw_resource {
protected:
    std::map<std::string, std::string> shaderPaths;

public:
    shaderProgram(const std::string& name, const std::map<std::string, std::string>& shaderPaths) :
        raw_resource(name), shaderPaths(shaderPaths) { }
    virtual ~shaderProgram() { }

    static shaderProgram* createShaderProgram(
        const std::string& name,
        const std::map<std::string, std::string>& shaderPaths);

    virtual void loadShaders(bool depthEnable = true, bool cullModeBackFaces = true) = 0;
    virtual void unloadShaders() = 0;
};