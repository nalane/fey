#pragma once

/*
 * Contains vaos and vbos for a model
 */
 
#include <map>
#include <vector>
#include <string>

#include "glHeaders.hpp"
#include "raw_resource.hpp"

class model : public raw_resource {
private:
  GLuint vao;
  std::map<int, GLuint> vbos;
  std::vector<GLuint> texIDs;
  int nonPassIndex;
  int elementsIndex;
  int elementsSize;
  int numVertices;

public:
  model(std::string name);
  ~model();

  void addData(GLenum target, GLsizeiptr size, void* data, GLenum usage, int shaderLocation = -1, int itemSize = 4);
  void setTexture(std::string source);
  void setVertices(std::vector<glm::vec3> vertexList);
  void setUVMapping(std::vector<glm::vec2> uvList);
  void setNormals(std::vector<glm::vec3> normalList);
  void setColors(std::vector<glm::vec4> colorList);
  void bindTextureToUniform(GLuint uniformID);

  void draw();
};
