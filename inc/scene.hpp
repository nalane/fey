#pragma once

/*
 * Contains base class for a generic scene
 */

#include "resourceHandler.hpp"
#include "object.hpp"
#include "glHeaders.hpp"
#include "physicsWorld.hpp"

#include <string>
#include <vector>
#include <map>

class scene {
protected:
  resource<shaderProgram> defaultShader;
  std::vector<bool> pressedKeys;
  std::map<std::string, object*> objects;

  // Physics world maintains the physical state of all objects in the scene
  physicsWorld world;
  
public:
  scene() : pressedKeys(std::vector<bool>(GLFW_KEY_LAST + 1, false)) {
    resourceHandler::getInstance()->loadShaderProg();
  }
  
  virtual ~scene() {
    for (auto p : objects) {
      delete p.second;
    }
  };

  // Generic scene interface
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

  virtual void updateObjects();

  virtual void drawObjects() {
    for (auto p : objects) {
      p.second->draw();
    }
  }
};
