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

  monkey suz;
  resource<firstPersonCamera> cam;
  resource<light> mainLight;
  resource<light> staticLight;
  
public:
  main_scene(resourceHandler* rHandler) : scene(rHandler), rad(0.0), suz(monkey(rHandler)) {};
  ~main_scene();

  void load();
  bool update();
  void draw();
  scene* nextScene();
};
