#pragma once

/*
 * A generic game object.
 */

#include "resourceHandler.hpp"
#include "shaderProgram.hpp"

#include <vector>
#include <string>

class object {
protected: 
	glm::mat4 modelMatrix;
	resourceHandler* rHandler;
  
	GLint texHandle;
	GLint progID;
	resource<shaderProgram> shaderProg;
	virtual void setShaderProg();
  
public:
	object(resourceHandler* rHandler) : rHandler(rHandler) {};
	virtual ~object();
				
	virtual void load();
	virtual void init();
	virtual void update();
	virtual void draw();

	virtual void translate(float x, float y, float z) {
		modelMatrix = glm::translate(glm::vec3(x, y, z)) * modelMatrix;
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
	virtual glm::mat4 getModelMatrix() { return modelMatrix; }
};
