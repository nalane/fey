#include <stdlib.h>

#include "graphicsBackend.h"
#include "log.h"

// Starts the graphics engine
graphicsBackend* initGraphics(int w, int h, char* windowTitle) {
  graphicsBackend* gfx = malloc(sizeof(graphicsBackend));
  
  if (!glfwInit()) {
    recordLog("FATAL ERROR: Could not initialize GLFW!");
    return NULL;
  }

  gfx->window = glfwCreateWindow(w, h, windowTitle, NULL, NULL);
  if (!gfx->window) {
    recordLog("FATAL ERROR: Could not create a GLFW window!");
    terminateGraphics(gfx);
    return NULL;
  }
  glfwMakeContextCurrent(gfx->window);

  glewExperimental = GL_TRUE;
  GLenum err = glewInit();
  if (err != GLEW_OK) {
    recordLog("FATAL ERROR: GLEW did not initialize correctly: %s", glewGetErrorString(err));
    return NULL;
  }

  return gfx;
}

// Performs drawing functions
void drawToGraphics(graphicsBackend* gfx, void (*drawFunction)()) {
  while (!glfwWindowShouldClose(gfx->window)) {
    drawFunction();
    glfwSwapBuffers(gfx->window);
    glfwPollEvents();
  }
}

// Quits GLFW and frees gfx
void terminateGraphics(graphicsBackend* gfx) {
  glfwTerminate();
  free(gfx);
}
