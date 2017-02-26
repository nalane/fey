#include "monkey.hpp"
#include "paths.hpp"

using namespace std;

monkey::monkey(resourceHandler* rHandler) : object(rHandler) {
}

monkey::~monkey() {

}

void monkey::load() {
  object::load();
  m = rHandler->loadModel("monkey.fey.model");
  modelMatrix = glm::rotate(glm::radians(-90.0f),
			    glm::vec3(1.0, 0.0, 0.0)) * modelMatrix;
}

void monkey::init() {
  object::init();
}

void monkey::update() {
  object::update();
  modelMatrix = glm::rotate(glm::radians(0.2f),
			    glm::vec3(0.0, 1.0, 0.0)) * modelMatrix;
}

void monkey::draw() {
  object::draw();
  m.res->bindTextureToUniform(texHandle);
  m.res->draw(progID);
}
