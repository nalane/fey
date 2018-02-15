#pragma once

/* 
 * Camera that allows first person perspective
 */

#include "camera.hpp"

class firstPersonCamera : public camera {
public:
  firstPersonCamera() : camera() { }
  firstPersonCamera(const glm::mat4& projection, const glm::mat4& view) : camera(projection, view) { }
  firstPersonCamera(const glm::vec3& pos, const glm::vec3& target, const glm::vec3& upVector,
		    float fieldOfView, float aspectRatio = defaultAspectRatio, float nearClip = 0.1, float farClip = 100.0) :
    camera(pos, target, upVector, fieldOfView, aspectRatio, nearClip, farClip) { }

  // Move distance units along a vector relative to the camera's current orientation
  void move(float distance, const glm::vec3& relativeDirection);
  void rotate(float degrees, const glm::vec3& axis);
};
