#pragma once

/*
 * An Axis-aligned bounding box for physics collisions
 */

#include "collisionShape.hpp"
#include "glHeaders.hpp"

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
  
  glm::vec3 getMin() const { return origin - halfExtents; }
  glm::vec3 getMax() const { return origin + halfExtents; }
  glm::vec3 getCenter() const { return origin; } 
};
