#pragma once

/*
 * Code for the monkey scene
 */

#include "firstPersonCamera.hpp"
#include "scene.hpp"
#include "monkey.hpp"

class main_scene : public scene {
private:
  float rad;

  firstPersonCamera* cam;
  
public:
  main_scene(resourceHandler* rHandler) : scene(rHandler), rad(0.0) {};
  ~main_scene();

  void load();
  bool update();
  void draw();
  std::string unload();
};
