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
  resource<skybox> sky;
  resource<light> staticLight;
  resource<trackBallCamera> cam;
  resource<shaderProgram> defaultShader;

  bool loggedMouseLocation;
  double mouseX;
  double mouseY;
  
public:
  second_scene(resourceHandler* rHandler) : scene(rHandler), loggedMouseLocation(false) {
    objects["c"] = new cube(rHandler);
    objects["p"] = new plane(rHandler);
  }
  ~second_scene();

  virtual void mousePosition(double xPos, double yPos);
  virtual void load();
  virtual bool update();
  virtual void draw();
  virtual scene* nextScene();
};
