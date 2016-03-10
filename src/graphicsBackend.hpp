#pragma once

/*
 * Contains fundamental functions for drawing to the screen
 */

#define GLEW_STATIC
#include <GL/glew.h>
#define GLFW_NO_GLU 1
#define GLFW_INCLUDE_COREARB 1
#include <GLFW/glfw3.h>

#include <string>

class graphicsBackend; // Resolves circular class dependencies
#include "engine.hpp"
#include "shaderProgram.hpp"

class graphicsBackend {
private:
  GLFWwindow* window;
  shaderProgram shaderProg;
  GLuint vao;

public:
  graphicsBackend(int w, int h, std::string windowTitle, std::string vertexSource, std::string fragmentSource);
  ~graphicsBackend();

  void drawToGraphics(engine* parentEngine);
};
