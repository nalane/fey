#pragma once

/*
 * Physics oriented collision box
 */

#include "glHeaders.hpp"
#include "physics/collisionShape.hpp"

#include <vector>

class collisionOBB : public collisionShape {
private:
  glm::mat4 coordinates;

  bool pointInBox(glm::vec3 point);

public:
  collisionOBB() : collisionShape(OBB) { }
  collisionOBB(glm::mat4 coordinates) : collisionShape(OBB), coordinates(coordinates) { }
  collisionOBB(std::vector<glm::vec3> vertices);

  void transform(glm::mat4 trans) { coordinates = trans * coordinates; }
};
