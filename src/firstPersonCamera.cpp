#include "firstPersonCamera.hpp"
#include "log.hpp"

using namespace std;

void firstPersonCamera::move(float amount, glm::vec3 localAxis) {
  glm::vec3 viewVector = target - position;
  glm::mat3 localCoords = glm::mat3(cross(viewVector, upVector), upVector, viewVector);
  glm::vec3 diff = amount * localCoords * localAxis;
  position += diff;
  target = position + viewVector;
  resetView();
}

void firstPersonCamera::rotate(float degrees, glm::vec3 axis) {
  glm::mat3 localCoords = glm::mat3(cross(target, upVector), upVector, target);
  axis = localCoords * axis;
  
  glm::mat4 rotMatrix = glm::rotate(degrees, axis);
  upVector = glm::vec3(rotMatrix * glm::vec4(upVector, 1.0));
  target = glm::vec3(rotMatrix * glm::vec4(target, 1.0));
  resetView();
}
