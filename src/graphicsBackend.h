#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

typedef struct {
  GLFWwindow* window;
} graphicsBackend;

graphicsBackend* initGraphics(int w, int h, char* windowTitle);
void drawToGraphics(graphicsBackend* gfx, void (*drawFunction)());
void terminateGraphics(graphicsBackend* gfx);
