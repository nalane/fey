#pragma once

/*
 * Contains model for a basic light
 */

#include "glHeaders.hpp"
#include "raw_resource.hpp"

class light : public raw_resource {
private:
  glm::vec4 position;
  glm::vec3 color;

public:
  light(std::string name, glm::vec4 position, glm::vec3 color) :
	raw_resource(name), position(position), color(color) {}

  void setPosition(glm::vec4 position) { this->position = position; }
  void setColor(glm::vec3 color) { this->color = color; }
  float* getPosition() { return &position[0]; }
  float* getColor() { return &color[0]; }
};
