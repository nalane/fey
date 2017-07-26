#include "plane.hpp"
#include "paths.hpp"
#include "physics/collisionPlane.hpp"

using namespace std;

plane::plane(resourceHandler* rHandler) : object(rHandler) {
}

plane::~plane() {

}

void plane::load() {
  object::load();
  mesh = rHandler->loadModel("plane.fey.model");
  rotate(-90.0, 1.0, 0.0, 0.0);
  scale(10.0);
}

void plane::init() {
  object::init();
  collider = new collisionPlane(glm::vec3(0, 1, 0), 0);
}

void plane::update() {
  object::update();
}

void plane::draw() {
  object::draw();
}
