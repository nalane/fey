#include "monkey.hpp"
#include "resourceHandler.hpp"

using namespace std;

monkey::monkey() : object() {
}

monkey::~monkey() {

}

void monkey::load() {
  object::load();
  mesh = resourceHandler::getInstance()->loadModel("monkey.fey.model");
  rotate(-90, 1, 0, 0);
  scale(1.3);
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
}
