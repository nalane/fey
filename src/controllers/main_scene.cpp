#include "main_scene.hpp"

using namespace std;

main_scene::~main_scene() {
  unload();
}

void main_scene::keyPress(int key, int action, int mods) {
  if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    switchScenes = true;
}

void main_scene::load() {
  objects["monkey"] = new monkey(rHandler);
  objects["monkey"]->load();
  objects["monkey"]->init();

  rHandler->setCamera("main", new camera(glm::vec3(3.0, 3.0,  3.0),
					 glm::vec3(0.0, 0.0,  0.0),
					 glm::vec3(0.0, 1.0,  0.0),
					 45.0));
  rHandler->setActiveCamera("main");
}

bool main_scene::update() {
  if (switchScenes) {
    switchScenes = false;
    return true;
  }
  
  objects["monkey"]->update();
  return false;
}

void main_scene::draw() {
  objects["monkey"]->draw();
}

string main_scene::unload() {
  for (auto p : objects) {
    delete p.second;
  }

  objects.clear();

  return "second";
}
