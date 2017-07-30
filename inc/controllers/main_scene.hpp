#pragma once

/*
 * Code for the monkey scene
 */

#include "firstPersonCamera.hpp"
#include "scene.hpp"
#include "monkey.hpp"
#include "skybox.hpp"

class main_scene : public scene {
private:
  float rad;

  resource<firstPersonCamera> cam;
  resource<light> mainLight;
  resource<light> staticLight;
  resource<skybox> sky;
  resource<shaderProgram> defaultShader;

  double mouseX;
  double mouseY;

  bool loggedMouseLocation;
  
public:
  main_scene(resourceHandler* rHandler) : scene(rHandler), rad(0.0),
					  loggedMouseLocation(false), mouseX(-1), mouseY(-1) {
              objects["suz"] = new monkey(rHandler);
            }
  ~main_scene();

  virtual void mousePosition(double xPos, double yPos);
  virtual void load();
  virtual bool update();
  virtual void draw();
  virtual scene* nextScene();
};
