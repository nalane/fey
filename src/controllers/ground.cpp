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
