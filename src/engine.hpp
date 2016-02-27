#pragma once

/*
 * Contains functions for initializing and running the game.
 */

#include <string>

#include "paths.hpp"
#include "graphicsBackend.hpp"

class engine {
private:
  int windowWidth;
  int windowHeight;
  std::string windowTitle;
  graphicsBackend* gfx;
  std::string vertexShader;
  std::string fragmentShader;
  GLuint vao;

  bool initGame();

public:
  engine(std::string configFile);
  ~engine();
  
  void runGame();
};
