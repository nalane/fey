#include "object.hpp"
#include "log.hpp"
#include "paths.hpp"
#include "feyModelLoader.hpp"

using namespace std;

// Loads the "object" into GPU memory
object::object(GLuint textureLocation) :
	textureLocation(textureLocation) {
  m = loadFeyModel(getFullPath("feyData/library/monkey.fey.model"));

  modelMatrix = glm::rotate(glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0)) * modelMatrix;
}

// Removes "object" from GPU
object::~object() {
  delete m;
}

// Tells GPU to render "object"
void object::draw() {
  modelMatrix = glm::rotate(glm::radians(0.2f), glm::vec3(0.0, 1.0, 0.0)) * modelMatrix;
  m->bindTextureToUniform(textureLocation);
  m->draw();
}
