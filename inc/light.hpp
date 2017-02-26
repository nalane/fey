#pragma once

/*
 * Contains model for a basic light
 */

#include "glHeaders.hpp"

class light {
private:
  glm::vec4 position;
  glm::vec3 color;

public:
  light(glm::vec4 position, glm::vec3 color) : position(position), color(color) {}

  void setPosition(glm::vec4 position) { this->position = position; }
  void setColor(glm::vec3 color) { this->color = color; }
  float* getPosition() { return &position[0]; }
  float* getColor() { return &color[0]; }
};
