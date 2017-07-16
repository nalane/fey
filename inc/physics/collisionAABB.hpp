#pragma once

/*
 * An Axis-aligned bounding box for physics collisions
 */

#include "collisionShape.hpp"
#include "glHeaders.hpp"
#include <vector>

class collisionAABB : public collisionShape {
private:
  glm::vec3 origin;
  glm::vec3 halfExtents;

public:
  collisionAABB() : collisionShape(AABB) { }
  collisionAABB(glm::vec3 origin, glm::vec3 halfExtents) :
    collisionShape(AABB),
    origin(origin),
    halfExtents(halfExtents) { }
  collisionAABB(std::vector<glm::vec3> vertices);
  
  glm::vec3 getMin() const { return origin - halfExtents; }
  glm::vec3 getMax() const { return origin + halfExtents; }
  glm::vec3 getCenter() const { return origin; }
  glm::vec3 getHalfExtents() const { return halfExtents; } 
};
