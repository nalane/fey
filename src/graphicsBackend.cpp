#include "graphicsBackend.hpp"
#include "shaders.hpp"
#include "log.hpp"

#include <string>

using namespace std;

// Starts the graphics engine
graphicsBackend::graphicsBackend(int w, int h, string windowTitle, string vertexSource, string fragmentSource) {
  // GLFW Initialization
  if (!glfwInit()) {
    recordLog("FATAL ERROR: Could not initialize GLFW!");
  }

  else {
    // Force use of OpenGL core
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // GLFW window creation
    window = glfwCreateWindow(w, h, windowTitle.c_str(), NULL, NULL);
    if (!window) {
      recordLog("FATAL ERROR: Could not create a GLFW window!");
      glfwTerminate();
    }

    else {
      glfwMakeContextCurrent(window);

      // Initialize extension wrangler with core extensions
      glewExperimental = GL_TRUE;
      GLenum err = glewInit();
      if (err != GLEW_OK) {
	string errMessage = string((const char*)glewGetErrorString(err));
	recordLog("FATAL ERROR: GLEW did not initialize correctly: " + errMessage);
      }

      else {
	// Enable features of openGL
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
      
	// Setup the shaders
	shaderProg = compileShaders(vertexSource, fragmentSource);
      }
    }
  }
}

// Quits GLFW and frees gfx
graphicsBackend::~graphicsBackend() {
  deleteShader(shaderProg);
  glfwTerminate();
}

// Performs drawing functions
void graphicsBackend::drawToGraphics(void (*drawFunction)()) {
  while (!glfwWindowShouldClose(window)) {
    glUseProgram(shaderProg);
    drawFunction();
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}
