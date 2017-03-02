#include "second_scene.hpp"

using namespace std;

second_scene::~second_scene() {
  unload();
}

void second_scene::keyPress(int key, int action, int mods) {
  if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    switchScenes = true;
}

void second_scene::load() {
  objects["monkey"] = new monkey(rHandler);
  objects["cube"] = new cube(rHandler);
  
  objects["monkey"]->load();
  objects["cube"]->load();
  
  objects["monkey"]->init();
  objects["cube"]->init();

  rHandler->setCamera("main", new camera(glm::vec3(-3.0, -3.0,  -3.0),
					 glm::vec3(0.0, 0.0,  0.0),
					 glm::vec3(0.0, 1.0,  0.0),
					 45.0));
  rHandler->setActiveCamera("main");
  
  lights["static"] = rHandler->loadLight("static");
  lights["static"].res->setPosition(glm::vec4(-1.0, 0.0, 0.0, 1.0));
  lights["static"].res->setColor(glm::vec3(1.0, 1.0, 1.0));
}

bool second_scene::update() {
  if (switchScenes) {
    switchScenes = false;
    return true;
  }

  for (auto p : objects) {
    p.second->update();
  }
  
  return false;
}

void second_scene::draw() {
  objects["cube"]->draw();
  objects["monkey"]->draw();
}

string second_scene::unload() {
  unloadResources();
  
  return "main";
}
