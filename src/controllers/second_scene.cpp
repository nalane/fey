#include "second_scene.hpp"

using namespace std;

second_scene::~second_scene() {
  unload();
}

void second_scene::load() {
  objects["monkey"] = new monkey(rHandler);
  objects["monkey"]->load();
  objects["monkey"]->init();

  rHandler->setCamera("main", new camera(glm::vec3(-3.0, -3.0,  -3.0),
					 glm::vec3(0.0, 0.0,  0.0),
					 glm::vec3(0.0, 1.0,  0.0),
					 45.0));
  rHandler->setActiveCamera("main");
}

bool second_scene::update() {
  if (key_state(GLFW_KEY_SPACE) == GLFW_PRESS)
    return true;
  
  objects["monkey"]->update();
  return false;
}

void second_scene::draw() {
  objects["monkey"]->draw();
}

string second_scene::unload() {
  for (auto p : objects) {
    delete p.second;
  }

  objects.clear();

  return "main";
}
