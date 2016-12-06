#pragma once

/*
 * Code for the monkey scene
 */

#include "scene.hpp"
#include "monkey.hpp"

class main_scene : public scene {
private:
  bool switchScenes;
  
public:
  main_scene(resourceHandler* rHandler) : scene(rHandler), switchScenes(false) {};
  ~main_scene();

  void keyPress(int key, int action, int mods);
  void load();
  bool update();
  void draw();
  std::string unload();
};
