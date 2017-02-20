#pragma once

/*
 * Code for the second monkey scene
 */

#include "scene.hpp"
#include "monkey.hpp"
#include "cube.hpp"

class second_scene : public scene {
private:
  bool switchScenes;
  
public:
  second_scene(resourceHandler* rHandler) : scene(rHandler), switchScenes(false) {};
  ~second_scene();

  void keyPress(int key, int action, int mods);
  void load();
  bool update();
  void draw();
  std::string unload();
};
