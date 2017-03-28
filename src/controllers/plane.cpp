#include "plane.hpp"
#include "paths.hpp"

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
}

void plane::update() {
  object::update();
}

void plane::draw() {
  object::draw();
}
