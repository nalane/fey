#include "shaderProgram.hpp"
#include "vkShaderProgram.hpp"
#include "glShaderProgram.hpp"

using namespace std;

// Helper function to create shader program based on what engine is in use
shaderProgram* shaderProgram::createShaderProgram(
    const std::string& name,
    const map<string, string>& shaderPaths) {
  GraphicsLibrary lib = graphics::getInstance()->getLibrary();
  if (lib == VULKAN) {
    return new vkShaderProgram(name, shaderPaths);
  }
  else if (lib == GL) {
    return new glShaderProgram(name, shaderPaths);
  }

  return nullptr;
}