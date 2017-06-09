#pragma once

/*
 * Camera object for viewing objects in a scene
 */

#include "glHeaders.hpp"
#include "raw_resource.hpp"

extern double defaultAspectRatio;
void setDefaultAspectRatio(double aspectRatio);

class camera : public raw_resource {
protected:
  glm::mat4 projection;
  glm::mat4 view;

  glm::vec3 position;
  glm::vec3 target;
  glm::vec3 upVector;

  virtual void resetView() { view = glm::lookAt(position, target, upVector); }

public:
  camera(const std::string& name);
  camera(const std::string& name, const glm::mat4& projection, const glm::mat4& view) : raw_resource(name), projection(projection), view(view) { }
  camera(const std::string& name, const glm::vec3& pos, const glm::vec3& target, const glm::vec3& upVector,
	 float fieldOfView, float aspectRatio = defaultAspectRatio, float nearClip = 0.1, float farClip = 100.0);
	 
  void setPosition(const glm::vec3& position) { this->position = position; resetView(); }
  void setTarget(const glm::vec3& target) { this->target = target; resetView(); }
  void setUpVector(const glm::vec3& upVector) { this->upVector = upVector; resetView(); }
  
  glm::mat4 getViewMatrix() const { return view; }
  glm::mat4 getProjectionMatrix() const { return projection; }
};
