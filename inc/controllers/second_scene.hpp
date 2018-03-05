#pragma once

/*
 * Code for the second scenewith the cube and plane
 */

#include "skybox.hpp"
#include "scene.hpp"
#include "cube.hpp"
#include "plane.hpp"

class second_scene : public scene {
private:
  // Resources
  resource<skybox> sky;

  // Mouse details
  bool loggedMouseLocation;
  double mouseX;
  double mouseY;
  
public:
  // Default constructor creates objects in scene
  second_scene() : scene(), loggedMouseLocation(false) {
    objects["c"] = new cube();
    objects["p"] = new plane();
  }
  ~second_scene();

  virtual void mousePosition(double xPos, double yPos);
  virtual void load();
  virtual UpdateResult update();
  virtual void draw();
  virtual scene* nextScene();
};
