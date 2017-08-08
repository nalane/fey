#pragma once

/*
 * A generic game object.
 */

#include "shaderProgram.hpp"
#include "model.hpp"
#include "collisionShape.hpp"
#include "resource.hpp"
#include "rigidBody.hpp"

#include <vector>

class object : public rigidBody {
protected: 
  glm::mat4 modelMatrix;
  
  std::vector<object*> children;
  object* parent;
  void addChild(object* child);
  void removeChildren();
  
  GLint texHandle;
  GLint progID;
  resource<model> mesh;
	
  resource<shaderProgram> shaderProg;
  virtual void setShaderProg();
  
public:
  object() : parent(nullptr), texHandle(-1), rigidBody() { }
  virtual ~object();
				
  virtual void load();
  virtual void init();
  virtual void update();
  virtual void draw();

  virtual void translate(float x, float y, float z);
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
  bool collidesWith(object* second);
};
