#pragma once

/*
 * Contains vaos and vbos for a model
 */
 
#include <map>
#include <vector>
#include <string>
#include "glHeaders.hpp"

class model {
private:
  GLuint vao;
  std::map<int, GLuint> vbos;
  std::vector<GLuint> texIDs;
  std::vector<glm::vec2> uvCoords;
  int nonPassIndex;
  int elementsIndex;
  int elementsSize;
  int numVertices;

public:
  model();
  ~model();
	
  void addData(GLenum target, GLsizeiptr size, void* data, GLenum usage, int shaderLocation = -1, int itemSize = 4);
  void setTexture(std::string source, std::vector<glm::vec2> uvCoords);
  void setVertices(std::vector<glm::vec3> vertexList);
  void setColors(std::vector<glm::vec4> colorList);
  void setElementIndices(std::vector<int> indexList);
  void setUVMapIndices(std::vector<int> indexList);
  void bindTextureToUniform(GLuint uniformID);
  void draw();
};
