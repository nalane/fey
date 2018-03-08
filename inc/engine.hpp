#pragma once

/*
 * Contains functions for initializing and running the game.
 */

#include <string>
#include <vector>

#include "glHeaders.hpp"
#include "scene.hpp"
#include "graphics.hpp"

class engine {
private:
  // Singleton
  static engine* instance;

  // Values from config file
  unsigned int windowWidth;
  unsigned int windowHeight;
  int numAASamples;
  bool hideCursor;
  bool fullscreen;
  std::string windowTitle;
  std::string vertexShader;
  std::string fragmentShader;

  bool initGraphics();
  bool initGame();
  
  void draw();

public:
  engine(const std::string& configFile);
  ~engine();

  static void createInstance(const std::string& configFile);
  static engine* getInstance();
  static void endInstance();

  std::string getWindowTitle() const {return windowTitle;}

  void runGame();
};