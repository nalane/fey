#include <math.h>
#include <time.h>

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

#include "engine.hpp"
#include "log.hpp"
#include "paths.hpp"
#include "main_scene.hpp"

using namespace std;

// GLFW Key press callbacks
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  engine::getInstance()->getActiveScene()->keyPress(key, action, mods);
}

void cursor_callback(GLFWwindow* window, double xpos, double ypos) {
  engine::getInstance()->getActiveScene()->mousePosition(xpos, ypos);
}

engine* engine::instance;

// Load the engine. Uses the values found in configFile
engine::engine(const string& configFile) {
  resourceHandler::createInstance();
  ifstream fin(configFile.c_str());
  if (fin.is_open()) {
    string dataPath, libraryPath;

    fin >> windowWidth;
    fin >> windowHeight;
    fin >> numAASamples;
    fin >> hideCursor;
    fin >> fullscreen;
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
  window = nullptr;

  delete activeScene;
  activeScene = nullptr;

  resourceHandler::endInstance();
}

// Create an instance of the engine
void engine::createInstance(const string& configFile) {
  instance = new engine(configFile);
}

// Return the instance of the engine
engine* engine::getInstance() {
  return instance;
}

// Delete the instance
void engine::endInstance() {
  delete instance;
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
  glfwWindowHint(GLFW_SAMPLES, numAASamples);

  // Determine if we should do it fullscreen
  GLFWmonitor* monitor = nullptr;
  if (fullscreen) {
    monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* videoMode = glfwGetVideoMode(monitor);
    windowWidth = videoMode->width;
    windowHeight = videoMode->height;
  }

  // Create the window
  window = glfwCreateWindow(windowWidth, windowHeight, windowTitle.c_str(), monitor, nullptr);
  if (!window) {
    recordLog("FATAL ERROR: Could not create a GLFW window!");
    glfwTerminate();
    return false;
  }

  // Set GLFW variables
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);
  glfwSetKeyCallback(window, key_callback);
  glfwSetCursorPosCallback(window, cursor_callback);
  if (hideCursor)
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // Init GL extensions
  if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
    recordLog("GLAD couldn't load extensions");
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

  enableGLFeatures();
  setDefaultAspectRatio((double) windowWidth / (double) windowHeight);
  return true;
}

// Prepares the game to run
bool engine::initGame() {
  srand(time(nullptr));
  if (!initGraphics())
    return false;

  resourceHandler::getInstance()->setDefaultShaderProg(vertexShader, fragmentShader);

  activeScene = new main_scene();
  activeScene->load();

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
    cerr << "A fatal error occurred. Check the log for more details." << endl;
  }
}
