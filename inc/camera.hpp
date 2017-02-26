#pragma once

/*
 * Camera object for viewing objects in a scene
 */

#include "glHeaders.hpp"

extern double defaultAspectRatio;
void setDefaultAspectRatio(double aspectRatio);

class camera {
private:
  glm::mat4 projection;
  glm::mat4 view;

public:
  camera();
  camera(glm::mat4 projection, glm::mat4 view) : projection(projection), view(view) {}
  camera(glm::vec3 pos, glm::vec3 target, glm::vec3 upVector,
	 float fieldOfView, float aspectRatio = defaultAspectRatio, float nearClip = 0.1, float farClip = 100.0);

  glm::mat4 getViewMatrix() { return view; }
  glm::mat4 getProjectionMatrix() { return projection; }
};
