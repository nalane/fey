#pragma once

#include <GL/glew.h>
#define GLFW_NO_GLU 1
#define GLFW_INCLUDE_COREARB 1
#include <GLFW/glfw3.h>

#include <string>

class graphicsBackend {
private:
  GLFWwindow* window;
  GLuint shaderProg;

public:
  graphicsBackend(int w, int h, std::string windowTitle, std::string vertexSource, std::string fragmentSource);
  ~graphicsBackend();

  void drawToGraphics(void (*drawFunction)());
};
