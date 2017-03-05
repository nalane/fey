#pragma once

/* 
 * Camera that allows first person perspective
 */

#include "camera.hpp"

class firstPersonCamera : public camera {
public:
  firstPersonCamera(std::string name) : camera(name) { }
  firstPersonCamera(std::string name, glm::mat4 projection, glm::mat4 view) : camera(name, projection, view) { }
  firstPersonCamera(std::string name, glm::vec3 pos, glm::vec3 target, glm::vec3 upVector,
	 float fieldOfView, float aspectRatio = defaultAspectRatio, float nearClip = 0.1, float farClip = 100.0) :
    camera(name, pos, target, upVector, fieldOfView, aspectRatio, nearClip, farClip) { }

  void move(float amount, glm::vec3 localAxis);
  void rotate(float degrees, glm::vec3 axis);
};
