#pragma once

/*
 * Contains functions for initializing and running the game.
 */

#include <string>
#include <vector>

class engine;
#include "resource.hpp"
#include "paths.hpp"
#include "object.hpp"
#include "camera.hpp"
#include "glHeaders.hpp"
#include "shaderProgram.hpp"

class engine {
private:
  int windowWidth;
  int windowHeight;
  std::string windowTitle;
  std::string vertexShader;
  std::string fragmentShader;
  std::vector<resource*> resources;
  camera* currentCamera;
  
  GLFWwindow* window;
  shaderProgram shaderProg;

  bool initGame();
  bool initGLFW();
  bool initGLEW();
  void enableGLFeatures();
  void setupShaders();
  bool initGraphics();

public:
  engine(std::string configFile);
  ~engine();

  void draw();
  void runGame();
};
