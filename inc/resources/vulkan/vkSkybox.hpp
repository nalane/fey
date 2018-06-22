#pragma once

/*
 * A skybox resource for Vulkan engine
 */

#include "raw_resource.hpp"
#include "shaderProgram.hpp"
#include "glHeaders.hpp"
#include "camera.hpp"
#include "skyboxVertex.hpp"
#include "texture.hpp"
#include "skybox.hpp"
#include "vulkan.hpp"

#include <string>

class vkSkybox : public skybox {
private:
  vulkan* graphicsEngine = nullptr;

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
  vkSkybox(const std::string& name);
  virtual ~vkSkybox();

  virtual void setTextures(texture* tex, std::string texturePaths[NUM_SKYBOX_TEXTURES]) {
    child_resources["textures"]["skybox"] = tex;
  }

  virtual void bindData();

  virtual void draw();
};
