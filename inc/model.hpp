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

public:
  model();
  ~model();
	
  void addData(GLenum target, GLsizeiptr size, void* data, GLenum usage, int shaderLocation = -1);
  void setElementIndices(std::vector<int> indexList);
  void draw();
};
