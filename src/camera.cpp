#include "camera.hpp"

using namespace std;

double defaultAspectRatio = 0;

void setDefaultAspectRatio(double aspectRatio) {
  defaultAspectRatio = aspectRatio;
}

// Sets a default perspective
camera::camera(string name) : raw_resource(name) {
  projection = glm::perspective(glm::radians(45.0), defaultAspectRatio, 0.1, 1000.0);
  view = glm::lookAt(glm::vec3(0.0, 0.0, 0.0),
		     glm::vec3(0.0, 0.0, 1.0),
		     glm::vec3(0.0, 1.0, 0.0));
}

// Set camera to specific perspective
camera::camera(string name, glm::vec3 pos, glm::vec3 target, glm::vec3 upVector,
	       float fieldOfView, float aspectRatio, float nearClip, float farClip) : raw_resource(name), position(pos), target(target), upVector(upVector) {
  view = glm::lookAt(pos, target, upVector);
  projection = glm::perspective(glm::radians(fieldOfView), aspectRatio, nearClip, farClip);
}
