#pragma once

/*
 * Contains vaos and vbos for a model
 */
 
#include <map>
#include <vector>
#include <string>

#include "glHeaders.hpp"
#include "model.hpp"
#include "material.hpp"
#include "texture.hpp"
#include "modelVertex.hpp"
#include "modelUniforms.hpp"
#include "shaderProgram.hpp"
#include "vulkan.hpp"

class vkModel : public model {
private:
  vulkan* graphicsEngine;

  VkDescriptorPool descriptorPool;
  VkDescriptorSet descriptorSet;

  VkBuffer vertexBuffer;
  VkDeviceMemory vertexBufferMemory;
  VkBuffer uniformBuffer;
  VkDeviceMemory uniformBufferMemory;

  VkCommandBuffer commandBuffer;

  bool verticesLoaded = false;
  bool descriptorsLoaded = false;

  void* mapping = nullptr;

public:
  vkModel(const std::string& name);
  ~vkModel();

  virtual void bindData();

  virtual void draw(modelUniforms uniforms);
};
