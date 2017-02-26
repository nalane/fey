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

  rHandler->setLight("main", new light(glm::vec4(2 * sin(rad), 0.0, 2 * cos(rad), 1.0), glm::vec3(1.0, 1.0, 1.0)));
  rHandler->setLight("static", new light(glm::vec4(0.0, 0.2, 0.0, 1.0), glm::vec3(1.0, 1.0, 1.0)));
}

bool main_scene::update() {
  if (switchScenes) {
    switchScenes = false;
    return true;
  }

  rad += 0.05;
  rHandler->getLight("main")->setPosition(glm::vec4(2 * sin(rad), 0.0, 2 * cos(rad), 1.0));
  
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
