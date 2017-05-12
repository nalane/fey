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
  light(const std::string& name, const glm::vec4& position, const glm::vec3& color) :
	raw_resource(name), position(position), color(color) {}

  void setPosition(const glm::vec4& position) { this->position = position; }
  void setColor(const glm::vec3& color) { this->color = color; }
  const float* getPosition() const { return &position[0]; }
  const float* getColor() const { return &color[0]; }
};
