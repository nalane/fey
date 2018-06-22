#pragma once

/*
 * Class contains and loads the shader program
 *  TODO: Make this class an abstract one that model and skybox inherit from
 */

#include <map>
#include <vector>

#include "shaderProgram.hpp"
#include "glHeaders.hpp"
#include "vulkan.hpp"

class vkShaderProgram : public shaderProgram {
private:
  vulkan* graphicsEngine;

  VkVertexInputBindingDescription bindingDescription;
  std::vector<VkVertexInputAttributeDescription> attributeDescriptions;

  VkDescriptorSetLayout descriptorSetLayout;

  VkPipelineLayout pipelineLayout;
  VkPipeline graphicsPipeline;

public:
  vkShaderProgram(const std::string& name, const std::map<std::string, std::string>& shaderPaths);
  ~vkShaderProgram();
  
  template <typename T>
  void setVertexAttributes();
  bool setDescriptorSetLayout();

  bool createVulkanDescriptorSet(VkDescriptorPool& descriptorPool, VkDescriptorSet& descriptorSet);

  virtual bool loadShaders(bool depthEnable = true, bool cullModeBackFaces = true);
  virtual void unloadShaders();

  VkPipeline getPipeline() { return graphicsPipeline; }
  VkPipelineLayout getPipelineLayout() { return pipelineLayout; }
};

template <typename T>
void vkShaderProgram::setVertexAttributes() {
  bindingDescription = T::getBindingDescription();
  attributeDescriptions = T::getAttributeDescriptions();
}
