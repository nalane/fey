#pragma once

/*
 * Camera object for viewing objects in a scene
 */

#include "glHeaders.hpp"

class camera {
private:
  glm::mat4 projection;
  glm::mat4 view;

public:
  camera();
  camera(glm::mat4 projection, glm::mat4 view) : projection(projection), view(view) {}
  camera(glm::vec3 pos, glm::vec3 target, glm::vec3 upVector,
	 float fieldOfView, float aspectRatio, float nearClip = 0.1, float farClip = 100.0);

  glm::mat4 getVPMatrix() {return projection * view;}
};
