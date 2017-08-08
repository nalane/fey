#pragma once

/*
 * Physics oriented collision box
 */

#include "glHeaders.hpp"
#include "collisionShape.hpp"

#include <vector>

class collisionOBB : public collisionShape {
private:
  glm::vec3 center;
  glm::vec3 halfExtents;

  bool pointInBox(glm::vec3 point);

public:
  collisionOBB() : collisionShape(OBB) { }
  collisionOBB(std::vector<glm::vec3> vertices);
};
