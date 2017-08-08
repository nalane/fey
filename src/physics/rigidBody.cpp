#include "rigidBody.hpp"

bool rigidBody::hasFiniteMass() {
  return inverseMass != 0.0;
}

double rigidBody::getMass() {
  return 1 / inverseMass;
}

void rigidBody::addForce(glm::vec3 force) {
  totForce += force;
  isAwake = true;
}

void rigidBody::updatePhysics(float duration) {
  if (hasFiniteMass()) {
    glm::vec3 acc = (1 / inverseMass) * totForce;
    velocity += duration * acc;
  }
  
  glm::vec3 deltaPos = duration * velocity;
  translate(deltaPos.x, deltaPos.y, deltaPos.z);
}
