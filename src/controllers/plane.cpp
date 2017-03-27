#include "plane.hpp"
#include "paths.hpp"

using namespace std;

plane::plane(resourceHandler* rHandler) : object(rHandler) {
}

plane::~plane() {

}

void plane::load() {
  object::load();
  m = rHandler->loadModel("plane.fey.model");
  modelMatrix = glm::rotate(glm::radians(-90.0f),
			    glm::vec3(1.0, 0.0, 0.0)) * modelMatrix;
  modelMatrix = glm::scale(glm::vec3(10.0, 10.0, 10.0)) * modelMatrix;
  modelMatrix = glm::translate(glm::vec3(0.0, 0.0, 0.0)) * modelMatrix;
}

void plane::init() {
  object::init();
}

void plane::update() {
  object::update();
}

void plane::draw() {
  object::draw();
  m.res->draw(progID);
}
