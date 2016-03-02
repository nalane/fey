#pragma once

/*
 * Contains functions for initializing and running the game.
 */

#include <string>

#include "paths.hpp"

class engine;
#include "object.hpp"
#include "graphicsBackend.hpp"

class engine {
private:
  int windowWidth;
  int windowHeight;
  std::string windowTitle;
  graphicsBackend* gfx;
  std::string vertexShader;
  std::string fragmentShader;
  object* obj;

  bool initGame();

public:
  engine(std::string configFile);
  ~engine();

  void draw();
  void runGame();
};
