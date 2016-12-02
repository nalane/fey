#pragma once

/*
 * Code for the monkey scene
 */

#include "scene.hpp"
#include "monkey.hpp"

class main_scene : public scene {
public:
  main_scene(resourceHandler* rHandler) : scene(rHandler) {};
  ~main_scene();

  void load();
  bool update();
  void draw();
  std::string unload();
};
