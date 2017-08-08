#include "cube.hpp"
#include "resourceHandler.hpp"
#include "paths.hpp"
#include "collisionAABB.hpp"

using namespace std;

cube::cube() : object() {
}

cube::~cube() {
}

void cube::load() {
  object::load();
  mesh = loadModel("cube.fey.model");
  collider = new collisionAABB(mesh.res->getVertices());
}

void cube::init() {
  object::init();
  translate(0, 2, 0);
  velocity = glm::vec3(0, -0.1, 0);
}

void cube::update() {
  object::update();
}

void cube::draw() {
  object::draw();
}
