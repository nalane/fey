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
  std::unordered_map<std::string, object*> objects;
  std::unordered_map<std::string, resource<light>> lights;
  std::unordered_map<std::string, resource<camera>> cameras;
  std::unordered_map<std::string, resource<firstPersonCamera>> firstPersonCameras;

  std::vector<bool> pressedKeys;
  
public:
  scene(resourceHandler* rHandler) : rHandler(rHandler), pressedKeys(std::vector<bool>(GLFW_KEY_LAST + 1, false)) {};
  virtual ~scene() {};

  virtual void keyPress(int key, int action, int mods);
  virtual void load() = 0;
  virtual bool update() = 0;
  virtual void draw() = 0;
  
  void unloadResources();
  virtual std::string unload() = 0;
};
