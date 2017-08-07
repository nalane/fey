#pragma once

/*
 * Contains base class for a generic scene
 */

#include "resourceHandler.hpp"
#include "object.hpp"
#include "light.hpp"
#include "camera.hpp"
#include "firstPersonCamera.hpp"
#include "paths.hpp"

#include <unordered_map>
#include <string>
#include <vector>
#include <map>

class scene {
protected:
  resource<shaderProgram> defaultShader;
  std::vector<bool> pressedKeys;
  std::map<std::string, object*> objects;
  
public:
  scene() : pressedKeys(std::vector<bool>(GLFW_KEY_LAST + 1, false)) {
    loadShaderProg();
  }
  
  virtual ~scene() {
    for (auto p : objects) {
      delete p.second;
    }
  };

  void keyPress(int key, int action, int mods);
  virtual void mousePosition(double xPos, double yPos) = 0;
  virtual scene* nextScene() = 0;
  virtual void load() = 0;
  virtual bool update() = 0;
  virtual void draw() = 0;

  virtual void loadObjects() {
    for (auto p : objects) {
      p.second->load();
      p.second->init();
    }
  }

  virtual void updateObjects() {
    for (auto p : objects) {
      p.second->update();
    }
  }

  virtual void drawObjects() {
    for (auto p : objects) {
      p.second->draw();
    }
  }
};
