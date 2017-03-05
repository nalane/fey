#pragma once

/*
 * Code for the monkey scene
 */

#include "firstPersonCamera.hpp"
#include "scene.hpp"
#include "monkey.hpp"

class main_scene : public scene {
private:
  bool switchScenes;
  float rad;

  firstPersonCamera* cam;
  
public:
  main_scene(resourceHandler* rHandler) : scene(rHandler), switchScenes(false), rad(0.0) {};
  ~main_scene();

  void keyPress(int key, int action, int mods);
  void load();
  bool update();
  void draw();
  std::string unload();
};
