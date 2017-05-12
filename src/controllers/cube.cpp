#include "cube.hpp"

using namespace std;

cube::cube(resourceHandler* rHandler) : object(rHandler) {
}

cube::~cube() {
}

void cube::load() {
  object::load();
  
  mesh = rHandler->loadModel("cube.fey.model");
}

void cube::init() {
  object::init();
}

void cube::update() {
  object::update();
  rotate(0.2, 1.0, 0.0, 0.0);
}

void cube::draw() {
  object::draw();
}
