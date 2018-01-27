#pragma once

/*
 * Contains functions for initializing and running the game.
 */

#include <string>

#include "glHeaders.hpp"
#include "scene.hpp"

class engine {
private:
  // Singleton
  static engine* instance;

  // Values from config file
  int windowWidth;
  int windowHeight;
  int numAASamples;
  bool hideCursor;
  bool fullscreen;
  std::string windowTitle;
  std::string vertexShader;
  std::string fragmentShader;

  // Rendering
  GLFWwindow* window;
  scene* activeScene;

  bool initGLFW();
  void enableGLFeatures();
  bool initGraphics();
  bool initGame();
  void draw();
  
public:
  engine(const std::string& configFile);
  ~engine();

  static void createInstance(const std::string& configFile);
  static engine* getInstance();
  static void endInstance();

  scene* getActiveScene() const { return activeScene; } 
  
  void runGame();
};

