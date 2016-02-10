#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

typedef struct {
  GLFWwindow* window;
  GLuint shaderProg;
} graphicsBackend;

graphicsBackend* initGraphics(int w, int h, char* windowTitle, char* vertexSource, char* fragmentSource);
void drawToGraphics(graphicsBackend* gfx, void (*drawFunction)());
void terminateGraphics(graphicsBackend* gfx);
