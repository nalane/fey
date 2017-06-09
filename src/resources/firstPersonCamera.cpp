#include "firstPersonCamera.hpp"
#include "log.hpp"

using namespace std;

void firstPersonCamera::move(float amount, const glm::vec3& localAxis) {
  glm::vec3 viewVector = target - position;
  glm::mat3 localCoords = glm::mat3(cross(viewVector, upVector), upVector, viewVector);
  glm::vec3 diff = amount * localCoords * localAxis;
  position += diff;
  target = position + viewVector;
  resetView();
}

void firstPersonCamera::rotate(float degrees, glm::vec3 axis) {
  glm::vec3 viewVector = target - position;
  glm::mat3 localCoords = glm::mat3(cross(viewVector, upVector), upVector, viewVector);
  axis = localCoords * axis;
  
  glm::mat4 rotMatrix = glm::rotate(degrees, axis);
  upVector = glm::vec3(rotMatrix * glm::vec4(upVector, 0.0));
  target = position + glm::vec3(rotMatrix * glm::vec4(viewVector, 0.0));
  resetView();
}
