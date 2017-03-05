#include <math.h>
#include <time.h>

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

#include "engine.hpp"
#include "log.hpp"
#include "paths.hpp"
#include "second_scene.hpp"

using namespace std;

// GLFW Key press call back can't be a method.
engine* runningGame = NULL;
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  runningGame->keyPress(key, action, mods);
}

// The constructor. Uses the values found in configFile
engine::engine(string configFile) {
  runningGame = this;
  ifstream fin(configFile.c_str());
  if (fin.is_open()) {
    string dataPath, libraryPath;

    fin >> windowWidth;
    fin >> windowHeight;
    fin.ignore(1, '\n');
    getline(fin, windowTitle);
    getline(fin, dataPath);
    getline(fin, libraryPath);
    getline(fin, vertexShader);
    getline(fin, fragmentShader);

    fin.close();

    recordLog("Values found in config file:");
    recordLog("----------------------------");
    recordLog("Window Width: " + to_string(windowWidth));
    recordLog("Window Height: " + to_string(windowHeight));
    recordLog("Window Title: " + windowTitle);
    recordLog("Data Path: " + dataPath);
    recordLog("Library Path: " + libraryPath);
    recordLog("Vertex Shader: " + vertexShader);
    recordLog("Fragment Shader: " + fragmentShader);

    setDataFolder(dataPath);
    setLibraryFolder(libraryPath);
	
    vertexShader = getDataFolderPath(vertexShader);
    fragmentShader = getDataFolderPath(fragmentShader);
  }

  else {
    recordLog("FATAL ERROR: Could not open config file " + configFile);
  }
}

// Destroys the game and all pointers used
engine::~engine() {
  glfwTerminate();

  delete activeScene;
  activeScene = NULL;
}

// Prepares the game to run
bool engine::initGame() {
  srand(time(NULL));
  if (!initGraphics())
    return false;

  shaderProg = rHandler.loadShaderProg(vertexShader, fragmentShader, true);
  shaderProg.res->useProgram();

  activeScene = new main_scene(&rHandler);
  activeScene->load();

  return true;
}

// GLFW Initialization
bool engine::initGLFW() {
  if (!glfwInit()) {
    recordLog("FATAL ERROR: Could not initialize GLFW!");
    return false;
  }

  // Force use of OpenGL core
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  // GLFW window creation
  window = glfwCreateWindow(windowWidth, windowHeight, windowTitle.c_str(), NULL, NULL);
  if (!window) {
    recordLog("FATAL ERROR: Could not create a GLFW window!");
    glfwTerminate();
    return false;
  }
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  glfwSetKeyCallback(window, key_callback);

  return true;
}

// Initialize extension wrangler with core extensions
bool engine::initGLEW() {
  glewExperimental = GL_TRUE;
  GLenum err = glewInit();
  if (err != GLEW_OK) {
    string errMessage = string((const char*)glewGetErrorString(err));
    recordLog("FATAL ERROR: GLEW did not initialize correctly: " + errMessage);
    return false;
  }
  
  return true;
}

// Enable features of openGL
void engine::enableGLFeatures() {
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glFrontFace(GL_CCW);
  glCullFace(GL_BACK);
}

// Start the graphics system
bool engine::initGraphics() {
  if (!initGLFW())
    return false;

  if (!initGLEW())
    return false;

  enableGLFeatures();
  setDefaultAspectRatio((double) windowWidth / (double) windowHeight);
  return true;
}

// Main drawing function
void engine::draw() {
  double currentTime = glfwGetTime();
  
  GLfloat color[] = {0.0, 0.0, 0.0, 1.0};
  glClearBufferfv(GL_COLOR, 0, color);
  glClear(GL_DEPTH_BUFFER_BIT);

  activeScene->draw();
}

// Run the game
void engine::runGame() {
  if (initGame()) {
    while (!glfwWindowShouldClose(window)) {
      if(activeScene->update()) {
	scene* nextScene = activeScene->nextScene();
	delete activeScene;
	activeScene = nextScene;
	activeScene->load();
	activeScene->update();
      }
      
      draw();
      glfwSwapBuffers(window);
      glfwPollEvents();
    }
  }

  else {
    cerr << "A fatal error occurred. Check the log for more details.<" << endl;
  }
}
