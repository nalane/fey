#include <math.h>
#include <time.h>

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <set>
#include <algorithm>

#include "engine.hpp"
#include "log.hpp"
#include "paths.hpp"
#include "main_scene.hpp"

using namespace std;

// Static members
engine* engine::instance;

// Load the engine. Uses the values found in configFile
engine::engine(const string& configFile) {
  ifstream fin(configFile.c_str());
  if (fin.is_open()) {
    string gLibrary, dataPath, libraryPath;

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
    fin >> gLibrary;
    graphicsEngine = gLibrary == "g" ? GL : VULKAN;

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
    recordLog("Graphics Engine: " + string(graphicsEngine == GL ? "OpenGL" : "Vulkan"));

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
  scene::endActiveScene();
  graphics::endInstance();
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

// Start the graphics system
bool engine::initGraphics() {
  resourceHandler::createInstance();
  if (!graphics::createInstance(graphicsEngine, fullscreen, windowWidth, windowHeight, windowTitle, hideCursor))
    return false;

  setDefaultAspectRatio((double) windowWidth / (double) windowHeight);
  return true;
}

// Prepares the game to run
bool engine::initGame() {
  srand(time(nullptr));
  if (!initGraphics())
    return false;

  resourceHandler::getInstance()->setDefaultShaderProg(vertexShader, fragmentShader);

  scene::setActiveScene(new main_scene());
  scene::getActiveScene()->load();

  return true;
}

// Main drawing function
void engine::draw() {
  return graphics::getInstance()->draw();
}

// Run the game
void engine::runGame() {
  // Make sure the game is initialized
  if (!initGame()) {
    cerr << "A fatal error occurred. Check the log for more details." << endl;
    return;
  }

  // Main game loop
  while (graphics::getInstance()->programShouldRun()) {
    if(scene::getActiveScene()->update() == CHANGE_SCENE) {
	    scene* nextScene = scene::getActiveScene()->nextScene();
      nextScene->load();

	    scene::endActiveScene();
      scene::setActiveScene(nextScene);
      nextScene->update();
    }
      
    draw();
    glfwPollEvents();
  }
}
