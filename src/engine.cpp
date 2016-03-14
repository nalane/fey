#include <math.h>
#include <time.h>

#include <string>
#include <fstream>
#include <iostream>

#include "engine.hpp"
#include "log.hpp"
#include "paths.hpp"

using namespace std;

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
  delete gfx;
  delete obj;
}

// Initializes the graphics system
bool engine::initGame() {
  srand(time(NULL));
  currentCamera = new camera(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, 1.0, 0.0),
	 45.0, (float)windowWidth / (float)windowHeight);
  
  gfx = new graphicsBackend(windowWidth, windowHeight, windowTitle, vertexShader, fragmentShader);

  float triPosition[3][4] = {
    { -0.4f, 0.1f, 1.0f, 1.0f},
    {  0.4f, 0.1f, 1.0f, 1.0f},
    {  0.0f, 0.7f, 1.0f, 1.0f},
  };

  float triColor[3][4] = {
    {1.0, 0.0, 0.0, 1.0},
    {0.0, 1.0, 0.0, 1.0},
    {0.0, 0.0, 1.0, 1.0}
  };
  obj = new object(triPosition, triColor);
  
  return (gfx != NULL);
}

// Main drawing function
void engine::draw() {
  double currentTime = glfwGetTime();
  
  GLfloat color[] = {0.0, 0.0, 0.0, 1.0};
  glClearBufferfv(GL_COLOR, 0, color);

  glm::mat4 transformMatrix = glm::mat4(1.0f);//currentCamera->getVPMatrix() * obj->getModelMatrix();
  GLint mvpHandle = glGetUniformLocation(gfx->getShaderProg()->getProgID(), "transformMatrix");
  glUniformMatrix4fv(mvpHandle, 1, GL_FALSE, &transformMatrix[0][0]);
  
  obj->draw();
}

// Run the game
void engine::runGame() {
  if (initGame())
    gfx->drawToGraphics(this);

  else
    cerr << "A fatal error occurred. Check the log for more details.<" << endl;
}
