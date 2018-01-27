#include "firstPersonCamera.hpp"
#include "log.hpp"

using namespace std;

void firstPersonCamera::move(float distance, const glm::vec3& relativeDirection) {
  // Figure out orientation vector
  glm::vec3 viewVector = target - position;
  glm::mat3 localCoords = glm::mat3(cross(viewVector, upVector), upVector, viewVector);

  // Determine translation vector
  glm::vec3 diff = distance * localCoords * relativeDirection;
  position += diff;
  target = position + viewVector;

  resetView();
}

void firstPersonCamera::rotate(float degrees, const glm::vec3& axis) {
  // Determine AOR in world space
  glm::vec3 viewVector = target - position;
  glm::mat3 localCoords = glm::mat3(cross(viewVector, upVector), upVector, viewVector);
  glm::vec3 globalAxis = localCoords * axis;
  
  // Reset target and upvector for rotation
  glm::mat4 rotMatrix = glm::rotate(degrees, globalAxis);
  upVector = glm::vec3(rotMatrix * glm::vec4(upVector, 0.0));
  target = position + glm::vec3(rotMatrix * glm::vec4(viewVector, 0.0));
  
  resetView();
}
