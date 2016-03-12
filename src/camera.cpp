#include "camera.hpp"

// Sets a default perspective
camera::camera() {
  projection = glm::perspective(45.0, 4.0 / 3.0, 0.1, 100.0);
  view = glm::lookAt(glm::vec3(0.0, 0.0, 0.0),
		     glm::vec3(0.0, 0.0, 1.0),
		     glm::vec3(0.0, 1.0, 0.0));
}

// Set camera to specific perspective
camera::camera(glm::vec3 pos, glm::vec3 target, glm::vec3 upVector,
	       float fieldOfView, float aspectRatio, float nearClip, float farClip) {
  view = glm::lookAt(pos, target, upVector);
  projection = glm::perspective(fieldOfView, aspectRatio, nearClip, farClip);
}
