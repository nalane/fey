#include "cube.hpp"
#include "paths.hpp"
#include "physics/collisionAABB.hpp"

using namespace std;

cube::cube(resourceHandler* rHandler) : object(rHandler) {
}

cube::~cube() {
}

void cube::load() {
  object::load();
  mesh = rHandler->loadModel("cube.fey.model");
  collider = new collisionAABB(mesh.res->getVertices());
}

void cube::init() {
  object::init();
  translate(0, 2, 0);
}

void cube::update() {
  object::update();
  translate(0, -0.01, 0);
}

void cube::draw() {
  object::draw();
}
