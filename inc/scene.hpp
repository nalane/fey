#pragma once

/*
 * Contains base class for a generic scene
 */

#include "resourceHandler.hpp"
#include "object.hpp"
#include "light.hpp"
#include "camera.hpp"
#include "firstPersonCamera.hpp"

#include <unordered_map>
#include <string>
#include <vector>

class scene {
protected:
  resourceHandler* rHandler;
  std::vector<bool> pressedKeys;
  
public:
  scene(resourceHandler* rHandler) : rHandler(rHandler), pressedKeys(std::vector<bool>(GLFW_KEY_LAST + 1, false)) {};
  virtual ~scene() {};

  virtual void keyPress(int key, int action, int mods);
  virtual void load() = 0;
  virtual bool update() = 0;
  virtual void draw() = 0;
 
  virtual scene* nextScene() = 0;
};
