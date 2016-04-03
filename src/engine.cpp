#include <math.h>
#include <time.h>

#include <vector>
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
    recordLog("Data Path: " + windowTitle);
    recordLog("Library Path: " + windowTitle);
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

  for (resource* obj : resources)
    delete obj;
}

// Initializes the graphics system
bool engine::initGame() {
  srand(time(NULL));
  if (!initGraphics())
    return false;
  
  currentCamera = new camera(glm::vec3(3.0, 3.0,  3.0),
			     glm::vec3(0.0, 0.0,  0.0),
			     glm::vec3(0.0, 1.0,  0.0),
			     45.0, (float)windowWidth / (float)windowHeight);  
  
  GLint texHandle = glGetUniformLocation(shaderProg.getProgID(), "texSampler");
  resources.push_back(new object(texHandle));

  for (resource* obj : resources) {
    obj->load();
    obj->init();
  }
  
  return true;
}

bool engine::initGLFW() {
  // GLFW Initialization
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

  return true;
}

bool engine::initGLEW() {
  // Initialize extension wrangler with core extensions
  glewExperimental = GL_TRUE;
  GLenum err = glewInit();
  if (err != GLEW_OK) {
    string errMessage = string((const char*)glewGetErrorString(err));
    recordLog("FATAL ERROR: GLEW did not initialize correctly: " + errMessage);
    return false;
  }
  
  return true;
}

void engine::enableGLFeatures() {
  // Enable features of openGL
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glFrontFace(GL_CCW);
  glCullFace(GL_BACK);
}

void engine::setupShaders() {
  shaderProg.addShader(new shader(vertexShader, GL_VERTEX_SHADER));
  shaderProg.addShader(new shader(fragmentShader, GL_FRAGMENT_SHADER));
  shaderProg.loadShaders();
  shaderProg.compileShaders();
  shaderProg.linkShaders();
  shaderProg.useProgram();
}

bool engine::initGraphics() {
  if (!initGLFW())
    return false;

  if (!initGLEW())
    return false;

  enableGLFeatures();
  setupShaders();
  return true;
}

// Main drawing function
void engine::draw() {
  double currentTime = glfwGetTime();
  
  GLfloat color[] = {0.0, 0.0, 0.0, 1.0};
  glClearBufferfv(GL_COLOR, 0, color);
  glClear(GL_DEPTH_BUFFER_BIT);

  for (resource* obj : resources) {
    glm::mat4 transformMatrix = currentCamera->getVPMatrix() * obj->getModelMatrix();
    GLint mvpHandle = glGetUniformLocation(shaderProg.getProgID(), "transformMatrix");
    glUniformMatrix4fv(mvpHandle, 1, GL_FALSE, &transformMatrix[0][0]);

    obj->update();
    obj->draw();
  }
}

// Run the game
void engine::runGame() {
  if (initGame()) {
    while (!glfwWindowShouldClose(window)) {
      shaderProg.useProgram();
      draw();
      glfwSwapBuffers(window);
      glfwPollEvents();
    }
  }

  else {
    cerr << "A fatal error occurred. Check the log for more details.<" << endl;
  }
}
