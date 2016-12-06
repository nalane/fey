#pragma once

/*
 * Contains functions for initializing and running the game.
 */

#include <string>
#include <vector>
#include <map>

class engine;
#include "paths.hpp"
#include "camera.hpp"
#include "glHeaders.hpp"
#include "shaderProgram.hpp"
#include "resourceHandler.hpp"
#include "resource.hpp"

// Game scenes
#include "main_scene.hpp"

class engine {
private:
  int windowWidth;
  int windowHeight;
  std::string windowTitle;
  std::string vertexShader;
  std::string fragmentShader;

  resourceHandler* rHandler;
  camera* currentCamera;
  resource<shaderProgram>* shaderProg;
  
  GLFWwindow* window;

  std::map<std::string, scene*> scenes;
  scene* activeScene;

  bool initGame();
  bool initGLFW();
  bool initGLEW();
  void enableGLFeatures();
  bool initGraphics();

public:
  engine(std::string configFile);
  ~engine();

  void draw();
  void runGame();
  
  void keyPress(int key, int action, int mods) {
    activeScene->keyPress(key, action, mods);
  }
};

#include "resource.cpp"
