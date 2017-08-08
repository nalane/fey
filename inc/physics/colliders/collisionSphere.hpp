#pragma once

/*
 * Sphere colliding shape
 */

#include "collisionShape.hpp"
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

  glm::vec3 getCenter() const { return center; }
  double getRadius() const { return radius; }

  virtual void translate(glm::vec3 diff) { center += diff; }
};
