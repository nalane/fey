#pragma once

/*
 * Vulkan implementation of terrain
 */

#include "terrain.hpp"
#include "vulkan.hpp"

class vkTerrain : public terrain {
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
  vkTerrain(const std::string& name);
  ~vkTerrain();

  virtual void bindData();
  virtual void draw(modelUniforms uniforms);
};