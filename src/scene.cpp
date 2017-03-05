#include "scene.hpp"

// Standard key press procedure
void scene::keyPress(int key, int action, int mods) {
  pressedKeys[key] = (action == GLFW_PRESS);
}

// Standard unload procedure
void scene::unloadResources() {
  for (auto p : objects) {
    delete p.second;
  }

  objects.clear();
  lights.clear();
  cameras.clear();
  firstPersonCameras.clear();
}
