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
#include "texture.hpp"
#include "modelVertex.hpp"
#include "modelUniforms.hpp"
#include "shaderProgram.hpp"

class model : public raw_resource {
private:
  std::vector<material> materials;
  std::vector<modelVertex> vertices;
  int numVertices;

  VkDescriptorPool descriptorPool;
  VkDescriptorSet descriptorSet;

  VkBuffer vertexBuffer;
  VkDeviceMemory vertexBufferMemory;
  VkBuffer uniformBuffer;
  VkDeviceMemory uniformBufferMemory;

  VkCommandBuffer commandBuffer;

  bool verticesLoaded;
  bool descriptorsLoaded;

  void* mapping = nullptr;

public:
  model(const std::string& name);
  ~model();

  void addMaterial(const material& mat);
  void setTexture(texture* tex);
  void setVertices(const std::vector<glm::vec3>& vertexList);
  void setUVMapping(const std::vector<glm::vec2>& uvList);
  void setNormals(const std::vector<glm::vec3>& normalList);
  void setShaderProgram(shaderProgram* shaderProg) {
    child_resources["shaderProgs"]["default"] = shaderProg;
  }

  void bindDescriptors();
  void bindVertices();

  void draw(modelUniforms uniforms);

  int getNumTextures() { return child_resources["textures"].size(); }
  std::vector<modelVertex> getVertices() const { return vertices; }
  std::vector<glm::vec3> getVertexPositions() const;
  material getMaterial() const { 
    // For now, shader only supports 1 material
    if (materials.size() > 0)
      return materials[0];
    return material(); 
  }
};
