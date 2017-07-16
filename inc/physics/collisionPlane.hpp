#pragma once

/*
 * A plane for collisions
 */

#include "collisionShape.hpp"
#include "glHeaders.hpp"

class collisionPlane : public collisionShape {
private:
  glm::vec3 normal;
  double distance;

public:
  collisionPlane() : collisionShape(PLANE) { }
  collisionPlane(glm::vec3 normal, double distance) :
    collisionShape(PLANE),
    normal(normal),
    distance(distance) { }
  collisionPlane(glm::vec3 normal, glm::vec3 pointOnPlane) :
    collisionShape(PLANE),
    normal(normal),
    distance(glm::dot(normal, pointOnPlane)) { }

  glm::vec3 getNormal() { return normal; }
  double getDistance() { return distance; }
};
