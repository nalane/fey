#include <stdlib.h>

#include "graphicsBackend.h"
#include "shaders.h"
#include "log.h"

// Starts the graphics engine
graphicsBackend* initGraphics(int w, int h, char* windowTitle, char* vertexSource, char* fragmentSource) {
  graphicsBackend* gfx = malloc(sizeof(graphicsBackend));

  // GLFW Initialization
  if (!glfwInit()) {
    recordLog("FATAL ERROR: Could not initialize GLFW!");
    return NULL;
  }

  // Force use of OpenGL core
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  // GLFW window creation
  gfx->window = glfwCreateWindow(w, h, windowTitle, NULL, NULL);
  if (!gfx->window) {
    recordLog("FATAL ERROR: Could not create a GLFW window!");
    terminateGraphics(gfx);
    return NULL;
  }
  glfwMakeContextCurrent(gfx->window);

  // Initialize extension wrangler with core extensions
  glewExperimental = GL_TRUE;
  GLenum err = glewInit();
  if (err != GLEW_OK) {
    recordLog("FATAL ERROR: GLEW did not initialize correctly: %s", glewGetErrorString(err));
    return NULL;
  }

  // Enable features of openGL
  glEnable(GL_CULL_FACE);
  glFrontFace(GL_CCW);
  glCullFace(GL_BACK);

  // Setup the shaders
  gfx->shaderProg = compileShaders(vertexSource, fragmentSource);
  
  return gfx;
}

// Performs drawing functions
void drawToGraphics(graphicsBackend* gfx, void (*drawFunction)()) {
  while (!glfwWindowShouldClose(gfx->window)) {
    glUseProgram(gfx->shaderProg);
    drawFunction();
    glfwSwapBuffers(gfx->window);
    glfwPollEvents();
  }
}

// Quits GLFW and frees gfx
void terminateGraphics(graphicsBackend* gfx) {
  deleteShader(gfx->shaderProg);
  glfwTerminate();
  free(gfx);
}
