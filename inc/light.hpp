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
  void setColor(glm::vec4 color) { this->color = color; }
  glm::vec4 getPosition() { return position; }
  glm::vec3 getColor() { return color; }
};
