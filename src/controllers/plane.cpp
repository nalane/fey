#include "plane.hpp"
#include "paths.hpp"

using namespace std;

plane::plane(resourceHandler* rHandler) : object(rHandler) {
}

plane::~plane() {

}

void plane::load() {
  object::load();
}

void plane::init() {
  object::init();
}

void plane::update() {
  object::update();
}

void plane::draw() {
  object::draw();
  m.res->bindTextureToUniform(texHandle);
  m.res->draw();
}
