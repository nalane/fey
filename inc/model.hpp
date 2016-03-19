#pragma once

/*
 * Contains vaos and vbos for a model
 */
 
#include <map>
#include <vector>
#include "glHeaders.hpp"

class model {
private:
  GLuint vao;
  std::map<int, GLuint> vbos;
  int nonPassIndex;
  int elementsIndex;
  int elementsSize;
  int numVertices;

public:
  model();
  ~model();
	
  void addData(GLenum target, GLsizeiptr size, void* data, GLenum usage, int shaderLocation = -1);
  void setVertices(std::vector<glm::vec3> vertexList);
  void setElementIndices(std::vector<int> indexList);
  void draw();
};
