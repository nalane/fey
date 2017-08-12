#pragma once

/*
 * A body that can have forces acted upon it
 */

#include "glHeaders.hpp"
#include "collisionShape.hpp"

class rigidBody {
protected:
  glm::vec3 velocity;
  glm::vec3 rotation;
  glm::vec3 totForce;
  float inverseMass;
  float linearDamping;
  bool isAwake;
  collisionShape* collider;

public:
  rigidBody(float inverseMass = 0.0, float linearDamping = 0.999) : inverseMass(inverseMass), linearDamping(linearDamping), collider(nullptr) { }

  bool hasFiniteMass();
  double getMass();

  void clearForce() { totForce = glm::vec3(0.0, 0.0, 0.0); }
  void addForce(glm::vec3 force);
  void updatePhysics(float duration);

  bool isColliding(rigidBody* rhs) { return collider->isColliding(rhs->collider); }

  glm::vec3 getMomentum() { return (1 / inverseMass) * velocity; }

  virtual void translate(float x, float y, float z) = 0;
  virtual void rotate(float degrees, float xAxis, float yAxis, float zAxis) = 0;
};
