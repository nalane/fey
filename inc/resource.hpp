#pragma once

#include "glHeaders.hpp"

class resource {

protected:
  glm::mat4 modelMatrix;

public:
  virtual ~resource();

  virtual void load() = 0; // Load resource data into main memory
  virtual void init() = 0; // Setup and process resource data for display
  virtual void update() = 0; // Each loop, perform an update for position, etc.
  virtual void draw() = 0; // Primary draw command

  virtual glm::mat4 getModelMatrix() { return modelMatrix; };
};
