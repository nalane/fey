#include "model.hpp"
#include "vkModel.hpp"
#include "glModel.hpp"
#include "log.hpp"

using namespace std;

// Helper function to create model based on what engine is in use
model* model::createModel(const std::string& name) {
  GraphicsLibrary lib = graphics::getInstance()->getLibrary();
  if (lib == VULKAN) {
    return new vkModel(name);
  }
  else if (lib == GL) {
    return new glModel(name);
  }

  return nullptr;
}

// Adds a material
void model::addMaterial (const material& mat) {
  materials.push_back(mat);
}

// Creates a texture for the GPU
void model::setTexture(texture* tex) {
  child_resources["textures"][tex->getName()] = tex;
}

vector<glm::vec3> model::getVertexPositions() const {
  vector<glm::vec3> positions;

  for (auto v : vertices) {
    positions.push_back(v.position);
  }

  return positions;
}