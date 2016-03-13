#pragma once

/*
 * Contains fundamental functions for drawing to the screen
 */

#include "glHeaders.hpp"
#include <string>

class graphicsBackend; // Resolves circular class dependencies
#include "engine.hpp"
#include "shaderProgram.hpp"
#include "camera.hpp"

class graphicsBackend {
private:
  GLFWwindow* window;
  shaderProgram shaderProg;
  GLuint vao;
  camera* currentCamera;

public:
  graphicsBackend(int w, int h, std::string windowTitle, std::string vertexSource, std::string fragmentSource);
  ~graphicsBackend();

  void drawToGraphics(engine* parentEngine);
  
  void setCamera(camera* c) : currentCamera(c) {};
};
