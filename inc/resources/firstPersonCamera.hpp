#pragma once

/* 
 * Camera that allows first person perspective
 */

#include "camera.hpp"

class firstPersonCamera : public camera {
public:
  firstPersonCamera(const std::string& name) : camera(name) { }
  firstPersonCamera(const std::string& name, const glm::mat4& projection, const glm::mat4& view) : camera(name, projection, view) { }
  firstPersonCamera(const std::string& name, const glm::vec3& pos, const glm::vec3& target, const glm::vec3& upVector,
		    float fieldOfView, float aspectRatio = defaultAspectRatio, float nearClip = 0.1, float farClip = 100.0) :
    camera(name, pos, target, upVector, fieldOfView, aspectRatio, nearClip, farClip) { }

  void move(float amount, const glm::vec3& localAxis);
  void rotate(float degrees, glm::vec3 axis);
};
