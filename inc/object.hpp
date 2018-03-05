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
  // Maintain object tree
  std::vector<object*> children;
  object* parent;
  void addChild(object* child);
  void removeChildren();

  // Data required for drawing
  glm::mat4 modelMatrix;
  resource<model> mesh;

  // Shader program for object
  resource<shaderProgram> shaderProg;
  virtual void setShaderProg();

public:
  object() : parent(nullptr), rigidBody() { }
  virtual ~object();

  // Standard interfact for all objects
  virtual void load();
  virtual void init();
  virtual void update();
  virtual void draw();

  // Transform object
  virtual void translate(float x, float y, float z);
  virtual void rotate(float degrees, float xAxis, float yAxis, float zAxis) {
    modelMatrix = glm::rotate(glm::radians(degrees),
			                        glm::vec3(xAxis, yAxis, zAxis))
                              * modelMatrix;
  }
  virtual void scale(float x, float y, float z) {
    modelMatrix = glm::scale(glm::vec3(x, y, z)) * modelMatrix;
  }
  virtual void scale(float allDimensions) {
    scale(allDimensions, allDimensions, allDimensions);
  }

  // Determine if this object collides with another
  bool collidesWith(object* second);
  
  virtual glm::mat4 getModelMatrix() const;
};
