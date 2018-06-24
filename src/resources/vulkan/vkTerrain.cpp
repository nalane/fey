#include "log.hpp"
#include "vkTerrain.hpp"
#include "graphics.hpp"
#include "vkShaderProgram.hpp"
#include "vkTexture.hpp"

using namespace std;

// Sets up the vao
vkTerrain::vkTerrain(const std::string& name) : terrain(name) {
  graphicsEngine = (vulkan*)graphics::getInstance();

  // Generate secondary command buffer
  VkCommandBufferAllocateInfo allocInfo = {};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.commandPool = graphicsEngine->getCommandPool();
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_SECONDARY;
  allocInfo.commandBufferCount = 1;

  VkResult result = vkAllocateCommandBuffers(graphicsEngine->getDevice(), &allocInfo, &commandBuffer);
  if (result != VK_SUCCESS) {
    recordLog("ERROR: Could not create secondary command buffer for " + name);
  }
}

// Removes all vbos and the vao
vkTerrain::~vkTerrain() {
  VkDevice device = graphicsEngine->getDevice();

  vkUnmapMemory(device, uniformBufferMemory);
  vkFreeMemory(device, uniformBufferMemory, nullptr);
  vkDestroyBuffer(device, uniformBuffer, nullptr);

  vkDestroyDescriptorPool(device, descriptorPool, nullptr);

  vkFreeMemory(device, vertexBufferMemory, nullptr);
  vkDestroyBuffer(device, vertexBuffer, nullptr);
}

void vkTerrain::bindData() {
  VkDevice device = graphicsEngine->getDevice();

  // Bind the vertices
  graphicsEngine->bindVertices(vertices, vertexBuffer, vertexBufferMemory);
  verticesLoaded = true;

  // Bind the descriptors
  VkDeviceSize bufferSize = sizeof(modelUniforms);
  graphicsEngine->createVulkanBuffer(bufferSize,
    VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
    uniformBuffer, uniformBufferMemory);
  vkMapMemory(device, uniformBufferMemory, 0, sizeof(modelUniforms), 0, &mapping);

  vkShaderProgram* prog = (vkShaderProgram*)child_resources["shaderProgs"]["default"];
  prog->createVulkanDescriptorSet(descriptorPool, descriptorSet);
  descriptorsLoaded = true;

  VkDescriptorBufferInfo bufferInfo = {};
  bufferInfo.buffer = uniformBuffer;
  bufferInfo.offset = 0;
  bufferInfo.range = sizeof(modelUniforms);

  vector<VkWriteDescriptorSet> descriptorWrites;
  descriptorWrites.push_back({});
  descriptorWrites.back().sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  descriptorWrites.back().dstSet = descriptorSet;
  descriptorWrites.back().dstBinding = 0;
  descriptorWrites.back().dstArrayElement = 0;
  descriptorWrites.back().descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  descriptorWrites.back().descriptorCount = 1;
  descriptorWrites.back().pBufferInfo = &bufferInfo;
  descriptorWrites.back().pImageInfo = nullptr;
  descriptorWrites.back().pTexelBufferView = nullptr;

  // One day we will likely allow multiple textures, but for now, just use one.
  if (child_resources["textures"].size() > 0) {
    vkTexture* tex = (vkTexture*)(child_resources["textures"].begin()->second);
    VkDescriptorImageInfo imageInfo = {};
    imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    imageInfo.imageView = tex->getImageView();
    imageInfo.sampler = tex->getSampler();

    descriptorWrites.push_back({});
    descriptorWrites.back().sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrites.back().dstSet = descriptorSet;
    descriptorWrites.back().dstBinding = 1;
    descriptorWrites.back().dstArrayElement = 0;
    descriptorWrites.back().descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    descriptorWrites.back().descriptorCount = 1;
    descriptorWrites.back().pImageInfo = &imageInfo;
  } 

  vkUpdateDescriptorSets(device, descriptorWrites.size(), descriptorWrites.data(), 0 , nullptr);
}

// Draws the model
void vkTerrain::draw(modelUniforms uniforms) { 
  // Make sure that model vertices were sent to GPU
  if (!verticesLoaded && !descriptorsLoaded) {
    bindData();
  }

  // Secondary buffer inheritance info
  VkCommandBufferInheritanceInfo inheritanceInfo = {};
  inheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
  inheritanceInfo.pNext = nullptr;
  inheritanceInfo.renderPass = graphicsEngine->getRenderPass();
  inheritanceInfo.subpass = 0;
  inheritanceInfo.framebuffer = graphicsEngine->getActiveFramebuffer();

  // Begin command buffer
  VkCommandBufferBeginInfo beginInfo = {};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  beginInfo.flags = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT | VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
  beginInfo.pInheritanceInfo = &inheritanceInfo;
  vkBeginCommandBuffer(commandBuffer, &beginInfo);

  // Activate shader
  vkShaderProgram* prog = (vkShaderProgram*)child_resources["shaderProgs"]["default"];
  vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, prog->getPipeline());
  vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, prog->getPipelineLayout(), 0, 1, &descriptorSet, 0, nullptr);

  // Send uniforms to GPU
  memcpy(mapping, &uniforms, sizeof(uniforms));

  // Give draw commands
  VkBuffer vertexBuffers[] = {vertexBuffer};
  VkDeviceSize offsets[] = {0};
  vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
  vkCmdDraw(commandBuffer, vertices.size(), 1, 0, 0);

  // End command buffer
  vkEndCommandBuffer(commandBuffer);

  // Send secondary buffer to primary buffer
  VkCommandBuffer activeBuffer = graphicsEngine->getActiveCommandBuffer();
  vkCmdExecuteCommands(activeBuffer, 1, &commandBuffer);
}
