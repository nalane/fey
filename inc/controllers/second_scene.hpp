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
  resource<trackBallCamera> cam;

  bool loggedMouseLocation;
  double mouseX;
  double mouseY;
  
public:
  second_scene(resourceHandler* rHandler) : scene(rHandler), suz(monkey(rHandler)), c(cube(rHandler)),
					    loggedMouseLocation(false) { }
  ~second_scene();

  void mousePosition(double xPos, double yPos);
  void load();
  bool update();
  void draw();
  scene* nextScene();
};
