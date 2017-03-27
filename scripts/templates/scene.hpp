#pragma once

/*
 * Code for the monkey scene
 */

#include "scene.hpp"

class @name@ : public scene {
private:
  double mouseX;
  double mouseY;
  
public:
  @name@(resourceHandler* rHandler) : scene(rHandler), mouseX(-1), mouseY(-1) { };
  ~@name@();

  void mousePosition(double xPos, double yPos);
  void load();
  bool update();
  void draw();
  scene* nextScene();
};
