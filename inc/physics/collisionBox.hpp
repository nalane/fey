#pragma once

/*
 * Physics collision box
 */

#include "glHeaders.hpp"
#include "physics/collisionShape.hpp"

#include <vector>

class collisionBox : public collisionShape {
private:
  glm::mat4 coordinates;

  bool pointInBox(glm::vec3 point);

public:
  collisionBox() : collisionShape() { }
  collisionBox(glm::mat4 coordinates) : collisionShape(), coordinates(coordinates) { }
  collisionBox(std::vector<glm::vec3> vertices);

  void transform(glm::mat4 trans) { coordinates = trans * coordinates; }

  virtual bool isColliding(collisionShape* second);
};
