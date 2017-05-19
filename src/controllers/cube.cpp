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
  translate(0.0, 25.0, 0.0);
  object::addPhysicsRigidBody();
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
