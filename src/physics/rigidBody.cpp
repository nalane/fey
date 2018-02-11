#include "rigidBody.hpp"

bool rigidBody::hasFiniteMass() const {
  return inverseMass != 0.0;
}

double rigidBody::getMass() const {
  return 1 / inverseMass;
}

void rigidBody::addForce(glm::vec3 force) {
  totForce += force;
  isAwake = true;
}

void rigidBody::updatePhysics(float duration) {
  const float EPSILON = 0.2f;
  if (hasFiniteMass()) {
    glm::vec3 acc = inverseMass * totForce;
    if (glm::length(acc) >= EPSILON)
      velocity += duration * acc;
  }

  if (glm::length(velocity) >= EPSILON) {
    glm::vec3 deltaPos = duration * velocity;
    translate(deltaPos.x, deltaPos.y, deltaPos.z);
  }
}
