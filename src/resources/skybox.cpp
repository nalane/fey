#include "skybox.hpp"
#include "resourceHandler.hpp"
#include "log.hpp"
#include "scene.hpp"
#include "graphics.hpp"
#include "skyboxUniforms.hpp"

using namespace std;

#define SIZE 333.3f

skybox::skybox(const string& name) : raw_resource(name), verticesLoaded(false), descriptorsLoaded(false) {
  // Generate vertices
  vertices = {
    // Positions
	  {{-SIZE,  SIZE, -SIZE, 1.0f}},
    {{-SIZE, -SIZE, -SIZE, 1.0f}},
    {{SIZE, -SIZE, -SIZE, 1.0f}},
    {{SIZE, -SIZE, -SIZE, 1.0f}},
    {{SIZE,  SIZE, -SIZE, 1.0f}},
    {{-SIZE,  SIZE, -SIZE, 1.0f}},
  
    {{-SIZE, -SIZE,  SIZE, 1.0f}},
    {{-SIZE, -SIZE, -SIZE, 1.0f}},
    {{-SIZE,  SIZE, -SIZE, 1.0f}},
    {{-SIZE,  SIZE, -SIZE, 1.0f}},
    {{-SIZE,  SIZE,  SIZE, 1.0f}},
    {{-SIZE, -SIZE,  SIZE, 1.0f}},
  
    {{SIZE, -SIZE, -SIZE, 1.0f}},
    {{SIZE, -SIZE,  SIZE, 1.0f}},
    {{SIZE,  SIZE,  SIZE, 1.0f}},
    {{SIZE,  SIZE,  SIZE, 1.0f}},
    {{SIZE,  SIZE, -SIZE, 1.0f}},
    {{SIZE, -SIZE, -SIZE, 1.0f}},
   
    {{-SIZE, -SIZE,  SIZE, 1.0f}},
    {{-SIZE,  SIZE,  SIZE, 1.0f}},
    {{SIZE,  SIZE,  SIZE, 1.0f}},
    {{SIZE,  SIZE,  SIZE, 1.0f}},
    {{SIZE, -SIZE,  SIZE, 1.0f}},
    {{-SIZE, -SIZE,  SIZE, 1.0f}},
  
    {{-SIZE,  SIZE, -SIZE, 1.0f}},
    {{SIZE,  SIZE, -SIZE, 1.0f}},
    {{SIZE,  SIZE,  SIZE, 1.0f}},
    {{SIZE,  SIZE,  SIZE, 1.0f}},
    {{-SIZE,  SIZE,  SIZE, 1.0f}},
    {{-SIZE,  SIZE, -SIZE, 1.0f}},
  
    {{-SIZE, -SIZE, -SIZE, 1.0f}},
    {{-SIZE, -SIZE,  SIZE, 1.0f}},
    {{SIZE, -SIZE, -SIZE, 1.0f}},
    {{SIZE, -SIZE, -SIZE, 1.0f}},
    {{-SIZE, -SIZE,  SIZE, 1.0f}},
    {{SIZE, -SIZE,  SIZE, 1.0f}},
  };

  // Generate secondary command buffer
  VkCommandBufferAllocateInfo allocInfo = {};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.commandPool = graphics::getInstance()->getCommandPool();
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_SECONDARY;
  allocInfo.commandBufferCount = 1;

  if (vkAllocateCommandBuffers(graphics::getInstance()->getDevice(), &allocInfo, &commandBuffer) != VK_SUCCESS) {
    recordLog("ERROR: Could not create secondary command buffer for " + name);
  }
}

// Remove data from GPU buffer
skybox::~skybox() {
  VkDevice device = graphics::getInstance()->getDevice();

  vkUnmapMemory(device, uniformBufferMemory);
  vkFreeMemory(device, uniformBufferMemory, nullptr);
  vkDestroyBuffer(device, uniformBuffer, nullptr);

  vkDestroyDescriptorPool(device, descriptorPool, nullptr);

  vkFreeMemory(device, vertexBufferMemory, nullptr);
  vkDestroyBuffer(device, vertexBuffer, nullptr);
}

// Bind the vertices
void skybox::bindVertices() {
  verticesLoaded = graphics::getInstance()->bindVertices(vertices, vertexBuffer, vertexBufferMemory);
}

// Bind the uniform descriptors
void skybox::bindDescriptors() {
  VkDevice device = graphics::getInstance()->getDevice();

  VkDeviceSize bufferSize = sizeof(skyboxUniforms);
  graphics::getInstance()->createVulkanBuffer(bufferSize,
    VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
    uniformBuffer, uniformBufferMemory);
  vkMapMemory(device, uniformBufferMemory, 0, sizeof(skyboxUniforms), 0, &mapping);

  texture* tex = (texture*)child_resources["textures"]["skybox"];
  shaderProgram* prog = (shaderProgram*)child_resources["shaderProgs"]["skybox"];
  descriptorsLoaded = prog->createVulkanDescriptorSet(descriptorPool, descriptorSet);

  VkDescriptorBufferInfo bufferInfo = {};
  bufferInfo.buffer = uniformBuffer;
  bufferInfo.offset = 0;
  bufferInfo.range = sizeof(skyboxUniforms);

  VkDescriptorImageInfo imageInfo = {};
  imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
  imageInfo.imageView = tex->getImageView();
  imageInfo.sampler = tex->getSampler();

  vector<VkWriteDescriptorSet> descriptorWrites(2);
  descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  descriptorWrites[0].dstSet = descriptorSet;
  descriptorWrites[0].dstBinding = 0;
  descriptorWrites[0].dstArrayElement = 0;
  descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  descriptorWrites[0].descriptorCount = 1;
  descriptorWrites[0].pBufferInfo = &bufferInfo;
  descriptorWrites[0].pImageInfo = nullptr;
  descriptorWrites[0].pTexelBufferView = nullptr;

  descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  descriptorWrites[1].dstSet = descriptorSet;
  descriptorWrites[1].dstBinding = 1;
  descriptorWrites[1].dstArrayElement = 0;
  descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
  descriptorWrites[1].descriptorCount = 1;
  descriptorWrites[1].pImageInfo = &imageInfo;

  vkUpdateDescriptorSets(device, descriptorWrites.size(), descriptorWrites.data(), 0 , nullptr);
}

void skybox::draw() {
  // Make sure that model vertices were sent to GPU
  if (!verticesLoaded) {
    bindVertices();
  }

  // Make sure that the uniform descriptors have been sent to the GPU
  if (!descriptorsLoaded) {
    bindDescriptors();
  }

    // Secondary buffer inheritance info
  VkCommandBufferInheritanceInfo inheritanceInfo = {};
  inheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
  inheritanceInfo.pNext = nullptr;
  inheritanceInfo.renderPass = graphics::getInstance()->getRenderPass();
  inheritanceInfo.subpass = 0;
  inheritanceInfo.framebuffer = graphics::getInstance()->getActiveFramebuffer();

  // Begin command buffer
  VkCommandBufferBeginInfo beginInfo = {};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  beginInfo.flags = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT | VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
  beginInfo.pInheritanceInfo = &inheritanceInfo;
  vkBeginCommandBuffer(commandBuffer, &beginInfo);

	// Activate Shader
  shaderProgram* prog = (shaderProgram*)(child_resources["shaderProgs"]["skybox"]);
  vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, prog->getPipeline());
  vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, prog->getPipelineLayout(), 0, 1, &descriptorSet, 0, nullptr);
  
  // Move the view matrices to the GPU
  camera* activeCamera = scene::getActiveScene()->getActiveCamera();
  glm::mat4 viewMatrix = glm::mat4(glm::mat3(activeCamera->getViewMatrix()));
  glm::mat4 vpMatrix = activeCamera->getProjectionMatrix() * viewMatrix;

  // Get uniforms
  skyboxUniforms uniforms = {};
  uniforms.vpMatrix = vpMatrix;

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
  VkCommandBuffer activeBuffer = graphics::getInstance()->getActiveCommandBuffer();
  vkCmdExecuteCommands(activeBuffer, 1, &commandBuffer);
}
