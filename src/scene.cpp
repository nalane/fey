#include "scene.hpp"

// Standard unload procedure
void scene::unloadResources() {
  for (auto p : objects) {
    delete p.second;
  }

  objects.clear();
  lights.clear();
}