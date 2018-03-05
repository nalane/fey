#pragma once

/*
 * Contains base class for a generic scene
 */

#include "resourceHandler.hpp"
#include "object.hpp"
#include "glHeaders.hpp"
#include "physicsWorld.hpp"
#include "light.hpp"

#include <string>
#include <vector>
#include <map>

enum UpdateResult {
  CHANGE_SCENE,
  NO_CHANGE
};

class scene {
private:
  static scene* activeScene;

protected:
  resource<shaderProgram> defaultShader;
  std::vector<bool> pressedKeys;
  std::map<std::string, object*> objects;
  std::map<std::string, light*> lights;
  std::map<std::string, camera*> cameras;

  // Physics world maintains the physical state of all objects in the scene
  physicsWorld world;

  std::string activeCameraID;
  
public:
  scene() : pressedKeys(std::vector<bool>(GLFW_KEY_LAST + 1, false)) {
    defaultShader = resourceHandler::getInstance()->loadShaderProg();
  }
  
  virtual ~scene();

  // Interact with active scene
  static scene* getActiveScene();
  static void setActiveScene(scene* newScene);
  static void endActiveScene();

  // Generic scene interface
  void keyPress(int key, int action, int mods);
  virtual void mousePosition(double xPos, double yPos) = 0;
  virtual scene* nextScene() = 0;
  virtual void load() = 0;
  virtual UpdateResult update() = 0;
  virtual void draw() = 0;

  void setActiveCamera(const std::string& id);
  camera* getActiveCamera();
  std::vector<light*> getAllLights();

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
