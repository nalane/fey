#pragma once

/*
 * Contains vaos and vbos for a model
 */
 
#include <map>
#include <vector>
#include <string>

#include "glHeaders.hpp"
#include "raw_resource.hpp"
#include "material.hpp"

class model : public raw_resource {
private:
  GLuint vao;
  std::map<int, GLuint> vbos;
  std::vector<GLuint> texIDs;
  std::vector<material> materials;
  std::vector<glm::vec3> vertices;
  int nonPassIndex;
  int elementsIndex;
  int elementsSize;
  int numVertices;

public:
  model(const std::string& name);
  ~model();

  void addMaterial(const material& mat);
  void addData(GLenum target, GLsizeiptr size, void* data, GLenum usage, int shaderLocation = -1, int itemSize = 4);
  void setTexture(const std::string& source);
  void setVertices(const std::vector<glm::vec3>& vertexList);
  void setUVMapping(const std::vector<glm::vec2>& uvList);
  void setNormals(const std::vector<glm::vec3>& normalList);
  void setColors(const std::vector<glm::vec4>& colorList);
  void bindTextureToUniform(GLuint uniformID);

  void draw(GLint progID);
};
