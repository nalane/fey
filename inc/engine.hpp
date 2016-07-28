#pragma once

/*
 * Contains functions for initializing and running the game.
 */

#include <string>
#include <vector>

class engine;
#include "paths.hpp"
#include "object.hpp"
#include "camera.hpp"
#include "glHeaders.hpp"
#include "shaderProgram.hpp"
#include "resourceHandler.hpp"

// Game objects
#include "monkey.hpp"

class engine {
private:
  int windowWidth;
  int windowHeight;
  std::string windowTitle;
  std::string vertexShader;
  std::string fragmentShader;

  resourceHandler* rHandler;
  std::vector<object*> objects;
  camera* currentCamera;
  
  GLFWwindow* window;

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
};
