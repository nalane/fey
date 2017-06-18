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
  int numAASamples;
  bool hideCursor;
  bool fullscreen;
  std::string windowTitle;
  std::string vertexShader;
  std::string fragmentShader;

  resourceHandler rHandler;
  resource<shaderProgram> shaderProg;
  
  GLFWwindow* window;

  scene* activeScene;

  bool initGame();
  bool initGLFW();
  void enableGLFeatures();
  bool initGraphics();

public:
  engine(const std::string& configFile);
  ~engine();

  void draw();
  void runGame();
  
  void keyPress(int key, int action, int mods) { activeScene->keyPress(key, action, mods); }
  void mousePosition(double xpos, double ypos) { activeScene->mousePosition(xpos, ypos); }
};
