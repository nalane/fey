#pragma once

/*
 * Code for the second monkey scene
 */

#include "skybox.hpp"
#include "scene.hpp"
#include "cube.hpp"
#include "plane.hpp"

class second_scene : public scene {
private:
  cube c;
  plane p;
  resource<skybox> sky;
  resource<light> staticLight;
  resource<trackBallCamera> cam;
  resource<shaderProgram> defaultShader;

  bool loggedMouseLocation;
  double mouseX;
  double mouseY;
  
public:
  second_scene(resourceHandler* rHandler) : scene(rHandler), loggedMouseLocation(false),
					    c(cube(rHandler)), p(plane(rHandler)) { }
  ~second_scene();

  void mousePosition(double xPos, double yPos);
  void load();
  bool update();
  void draw();
  scene* nextScene();
};
