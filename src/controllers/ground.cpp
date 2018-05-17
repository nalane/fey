#include "ground.hpp"
#include "paths.hpp"

using namespace std;

ground::ground() : object() {
}

ground::~ground() {

}

void ground::load() {
  object::load();
  terrain = resourceHandler::getInstance()->loadTerrain("terrains/level1.fey.terrain");
  scale(10.0, 10.0, 0.5);
  rotate(-90, 1, 0, 0);
}

void ground::init() {
  object::init();
}

void ground::update() {
  object::update();
}

void ground::draw() {
  object::draw();
}
