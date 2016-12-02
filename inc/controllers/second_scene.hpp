#pragma once

/*
 * Code for the second monkey scene
 */

#include "scene.hpp"
#include "monkey.hpp"

class second_scene : public scene {
public:
  second_scene(resourceHandler* rHandler) : scene(rHandler) {};
  ~second_scene();

  void load();
  bool update();
  void draw();
  std::string unload();
};
