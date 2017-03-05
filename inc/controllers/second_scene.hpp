#pragma once

/*
 * Code for the second monkey scene
 */

#include "scene.hpp"
#include "monkey.hpp"
#include "cube.hpp"

class second_scene : public scene {
private:
  monkey suz;
  cube c;
  resource<light> staticLight;
  
public:
  second_scene(resourceHandler* rHandler) : scene(rHandler), suz(monkey(rHandler)), c(cube(rHandler)) { }
  ~second_scene();

  void load();
  bool update();
  void draw();
  scene* nextScene();
};
