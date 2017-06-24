#pragma once

/*
 * Sphere colliding shape
 */

#include "physics/collisionShape.hpp"
#include "glHeaders.hpp"

#include <vector>

class collisionSphere : public collisionShape {
private:
  glm::vec3 center;
  double radius;

public:
  collisionSphere() : collisionShape(SPHERE) { }
  collisionSphere(glm::vec3 center, double radius) : center(center), radius(radius), collisionShape(SPHERE) { }
  collisionSphere(std::vector<glm::vec3> points);

  virtual bool isColliding(collisionShape* second);
};
