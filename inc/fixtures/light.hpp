#pragma once

/*
 * Contains model for a basic light
 */

#include "glHeaders.hpp"
#include "raw_resource.hpp"

class light {
private:
  glm::vec4 position;
  glm::vec3 color;

public:
  light() { }
  light(const glm::vec4& position, const glm::vec3& color) : position(position), color(color) {}

  void setPosition(const glm::vec4& position) { this->position = position; }
  void setColor(const glm::vec3& color) { this->color = color; }
  float* getPosition() { return &position[0]; }
  float* getColor() { return &color[0]; }
};
