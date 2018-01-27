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
  // Measure rotation
  float rad;

  // Resources used in this scene
  resource<firstPersonCamera> cam;
  resource<light> mainLight;
  resource<light> staticLight;
  resource<skybox> sky;

  // Mouse position
  double mouseX;
  double mouseY;
  bool loggedMouseLocation;
  
public:
  // Default constructor sets up scene objects
  main_scene() : scene(), rad(0.0),
                 loggedMouseLocation(false), mouseX(-1), mouseY(-1) {
    objects["suz"] = new monkey();
  }
  ~main_scene();

  virtual void mousePosition(double xPos, double yPos);
  virtual void load();
  virtual bool update();
  virtual void draw();
  virtual scene* nextScene();
};
