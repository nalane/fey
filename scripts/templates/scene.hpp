#pragma once

/*
 * Code for the monkey scene
 */

#include "scene.hpp"

class @name@ : public scene {
private:
  bool switchScenes;
  
public:
  @name@(resourceHandler* rHandler) : scene(rHandler), switchScenes(false) {};
  ~@name@();

  void keyPress(int key, int action, int mods);
  void load();
  bool update();
  void draw();
  std::string unload();
};
