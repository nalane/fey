#include "main_scene.hpp"

using namespace std;

main_scene::~main_scene() {
  unload();
}

void main_scene::load() {
  objects["monkey"] = new monkey(rHandler);
  objects["monkey"]->load();
  objects["monkey"]->init();
}

void main_scene::update() {
  objects["monkey"]->update();
}

void main_scene::draw() {
  objects["monkey"]->draw();
}

void main_scene::unload() {
  for (auto p : objects) {
    delete p.second;
  }

  objects.clear();
}
