#pragma once

/*
 * A generic game object.
 */

#include "resourceHandler.hpp"
#include "shaderProgram.hpp"
#include "model.hpp"
#include "physics/collisionShape.hpp"

#include <vector>
#include <string>

class object {
protected: 
  glm::mat4 modelMatrix;
  resourceHandler* rHandler;
  
  std::vector<object*> children;
  object* parent;
  void addChild(object* child);
  void removeChildren();
  
  GLint texHandle;
  GLint progID;
  resource<model> mesh;
  collisionShape* collider;
	
  resource<shaderProgram> shaderProg;
  virtual void setShaderProg();
  
public:
  object(resourceHandler* rHandler) : rHandler(rHandler), parent(nullptr), texHandle(-1), collider(nullptr) { }
  virtual ~object();
				
  virtual void load();
  virtual void init();
  virtual void update();
  virtual void draw();

  virtual void translate(float x, float y, float z) {
    modelMatrix = glm::translate(glm::vec3(x, y, z)) * modelMatrix;
    collider->translate(x, y, z);
  }
  
  virtual void rotate(float degrees, float xAxis, float yAxis, float zAxis) {
    modelMatrix = glm::rotate(glm::radians(degrees),
			      glm::vec3(xAxis, yAxis, zAxis)) * modelMatrix;
  }
  
  virtual void scale(float x, float y, float z) {
    modelMatrix = glm::scale(glm::vec3(x, y, z)) * modelMatrix;
  }
  
  virtual void scale(float allDimensions) {
    scale(allDimensions, allDimensions, allDimensions);
  }
  
  virtual glm::mat4 getModelMatrix() const;
  bool collidesWith(const object& second);
};
