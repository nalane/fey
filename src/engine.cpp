#include <stdio.h>
#include <math.h>
#include <time.h>

#include <string>
#include <fstream>

#include "engine.hpp"
#include "log.hpp"
#include "paths.hpp"

using namespace std;

// Primary drawing function. For now, is experimental
void drawingTest() {
  double currentTime = glfwGetTime();
  
  GLfloat color[] = {0.0, 0.0, 0.0, 1.0};
  glClearBufferfv(GL_COLOR, 0, color);

  GLfloat attrib0[] = { (float)sin(currentTime) * 0.5f,
			(float)cos(currentTime) * 0.6f,
			0.0f, 0.0f};
  glVertexAttrib4fv(0, attrib0);

  GLfloat attrib1[] = { (float)rand() / RAND_MAX,
			(float)rand() / RAND_MAX,
			(float)rand() / RAND_MAX, 0.0f};
  glVertexAttrib4fv(1, attrib1);

  glDrawArrays(GL_TRIANGLES, 0, 3);
}

// The constructor. Uses the values found in configFile
engine::engine(string configFile) {
  ifstream fin(configFile.c_str());
  if (fin.is_open()) {
    fin >> windowWidth;
    fin >> windowHeight;
    fin.ignore(1, '\n');
    getline(fin, windowTitle);
    getline(fin, vertexShader);
    getline(fin, fragmentShader);
    fin.close();

    recordLog("Values found in config file:");
    recordLog("----------------------------");
    recordLog("windowWidth: " + to_string(windowWidth));
    recordLog("windowHeight: " + to_string(windowHeight));
    recordLog("windowTitle: " + windowTitle);
    recordLog("vertexShader: " + vertexShader);
    recordLog("fragmentShader: " + fragmentShader);

    vertexShader = getFullPath(vertexShader);
    fragmentShader = getFullPath(fragmentShader);
  }

  else {
    recordLog("FATAL ERROR: Could not open config file " + configFile);
  }
}

// Destroys the game and all pointers used
engine::~engine() {
  glDeleteVertexArrays(1, &vao);
  delete gfx;
}

// Initializes the graphics system
bool engine::initGame() {
  srand(time(NULL));
  
  gfx = new graphicsBackend(windowWidth, windowHeight, windowTitle, vertexShader, fragmentShader);
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  
  return (gfx != NULL);
}

// Run the game
void engine::runGame() {
  if (initGame())
    gfx->drawToGraphics(drawingTest);

  else
    fprintf(stderr, "A fatal error occurred. Check the log for more details.\n");
}
