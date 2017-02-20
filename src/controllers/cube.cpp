#include "cube.hpp"
#include "paths.hpp"

using namespace std;

cube::cube(resourceHandler* rHandler) : object(rHandler) {
}

cube::~cube() {

}

void cube::load() {
  object::load();
  m = rHandler->loadModel("cube.fey.model");
}

void cube::init() {
  object::init();
}

void cube::update() {
  object::update();
}

void cube::draw() {
  object::draw();
  m.res->bindTextureToUniform(texHandle);
  m.res->draw();
}
