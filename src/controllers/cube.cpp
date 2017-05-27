#include "cube.hpp"

using namespace std;

cube::cube(resourceHandler* rHandler) : object(rHandler) {
  mass = 1.0;
}

cube::~cube() {
}

void cube::load() {
  object::load();
  mesh = rHandler->loadModel("cube.fey.model");
  translate(5.0, 25.0, 5.0);
  object::addPhysicsRigidBody("hull");
}

void cube::init() {
  object::init();
}

void cube::update() {
  object::update();
}

void cube::draw() {
  object::draw();
}
