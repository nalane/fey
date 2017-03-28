#include "monkey.hpp"
#include "paths.hpp"

using namespace std;

monkey::monkey(resourceHandler* rHandler) : object(rHandler) {
}

monkey::~monkey() {

}

void monkey::load() {
  object::load();
  m = rHandler->loadModel("monkey.fey.model");
  rotate(-90, 1, 0, 0);
}

void monkey::init() {
  object::init();
}

void monkey::update() {
  object::update();
  rotate(0.2, 0, 1, 0);
}

void monkey::draw() {
  object::draw();
  m.res->bindTextureToUniform(texHandle);
  m.res->draw(progID);
}
