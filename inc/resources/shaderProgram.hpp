#pragma once

/*
 * Class contains and loads the shader program
 *  TODO: Make this class an abstract one that model and skybox inherit from
 */

#include <map>
#include <vector>

#include "raw_resource.hpp"
#include "glHeaders.hpp"

class shaderProgram : public raw_resource {
private:
  std::map<std::string, std::string> shaderPaths;

  VkVertexInputBindingDescription bindingDescription;
  std::vector<VkVertexInputAttributeDescription> attributeDescriptions;

  VkDescriptorSetLayout descriptorSetLayout;

  VkPipelineLayout pipelineLayout;
  VkPipeline graphicsPipeline;

public:
  shaderProgram(const std::string& name, const std::map<std::string, std::string>& shaderPaths) : raw_resource(name), shaderPaths(shaderPaths) { }
  ~shaderProgram();
  
  template <typename T>
  void setVertexAttributes();
  bool setDescriptorSetLayout();

  bool createVulkanDescriptorSet(VkDescriptorPool& descriptorPool, VkDescriptorSet& descriptorSet);

  bool loadShaders(VkBool32 depthEnable = VK_TRUE, VkCullModeFlags cullMode = VK_CULL_MODE_BACK_BIT);
  void unloadShaders();

  VkPipeline getPipeline() { return graphicsPipeline; }
  VkPipelineLayout getPipelineLayout() { return pipelineLayout; }
};

template <typename T>
void shaderProgram::setVertexAttributes() {
  bindingDescription = T::getBindingDescription();
  attributeDescriptions = T::getAttributeDescriptions();
}
