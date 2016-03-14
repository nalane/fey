#include "model.hpp"

using namespace std;

model::model() {
	glGenVertexArrays(1, &vao);
	nonPassIndex = -2;
	elementsIndex = 0;
}

model::~model() {
	glDeleteVertexArrays(1, &vao);
	for (pair<int, GLuint> p : vbos)
		glDeleteBuffers(1, &(p.second));
}

void model::addData(GLenum target, GLsizeiptr size, void* data, GLenum usage, int shaderLocation) {
	if (shaderLocation < 0) {
		shaderLocation = nonPassIndex;
		nonPassIndex--;
	}
	
	if (target == GL_ELEMENT_ARRAY_BUFFER) {
		elementsIndex = shaderLocation;
	}
	
	glBindVertexArray(vao);
	
	vbos[shaderLocation] = -1;
	glGenBuffers(1, &(vbos[shaderLocation]));
	
	glBindBuffer(target, vbos[shaderLocation]);
  glBufferData(target, size, data, usage);
  
  if (shaderLocation >= 0) {
	glEnableVertexAttribArray(shaderLocation);
	glVertexAttribPointer(shaderLocation, 4, GL_FLOAT, GL_FALSE, 0, 0);
  }
}

void model::setElementIndices(vector<int> indexList) {
	if (elementsIndex == 0) {
		addData(GL_ELEMENT_ARRAY_BUFFER, indexList.size() * sizeof(int), &indexList[0], GL_STATIC_DRAW);
		elementsSize = indexList.size();
	}
	
	else {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[elementsIndex]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexList.size() * sizeof(int), &indexList[0], GL_STATIC_DRAW);
		elementsSize = indexList.size();
	}
}

void model::draw() {
	glBindVertexArray(vao);
	
	if (elementsIndex < 0) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[elementsIndex]);
		glDrawElements(GL_TRIANGLES, elementsSize, GL_UNSIGNED_INT, NULL);
	}
}