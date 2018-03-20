#include "shaderProgram.hpp"
#include "vkShaderProgram.hpp"

using namespace std;

shaderProgram* shaderProgram::createShaderProgram(
        const string& name,
        const map<string, string>& shaderPaths) {
    if (graphics::getInstance()->getLibrary() == VULKAN) {
        return new vkShaderProgram(name, shaderPaths);
    }

    return nullptr;
}
