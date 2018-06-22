#pragma once

/*
 * Contains vaos and vbos for a model
 */

#include "model.hpp"
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
