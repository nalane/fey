#pragma once

/*
 * A skybox resource
 */

#include "raw_resource.hpp"
#include "shaderProgram.hpp"
#include "glHeaders.hpp"
#include "camera.hpp"
#include "skyboxVertex.hpp"
#include "texture.hpp"

#include <string>

#define SKYBOX_RIGHT        0
#define SKYBOX_LEFT         1
#define SKYBOX_TOP          2
#define SKYBOX_BOTTOM       3
#define SKYBOX_BACK         4
#define SKYBOX_FRONT        5
#define NUM_SKYBOX_TEXTURES 6

class skybox : public raw_resource {
private:
  std::vector<skyboxVertex> vertices;

  VkDescriptorPool descriptorPool;
  VkDescriptorSet descriptorSet;

  VkBuffer vertexBuffer;
  VkDeviceMemory vertexBufferMemory;
  VkBuffer uniformBuffer;
  VkDeviceMemory uniformBufferMemory;
  
  bool verticesLoaded;
  bool descriptorsLoaded;
  void* mapping = nullptr;
  
public:
  skybox(const std::string& name);
  ~skybox();

  void setShaderProgram(shaderProgram* shaderProg) {
    child_resources["shaderProgs"]["skybox"] = shaderProg;
  }
  void setTextures(texture* tex) {
    child_resources["textures"]["skybox"] = tex;
  }

  void bindVertices();
  void bindDescriptors();

  void draw();
};
