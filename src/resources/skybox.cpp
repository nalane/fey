#include "skybox.hpp"
#include "resourceHandler.hpp"
#include "log.hpp"
#include "scene.hpp"
#include "engine.hpp"
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
}

// Remove data from GPU buffer
skybox::~skybox() {
  VkDevice device = engine::getInstance()->getDevice();

  vkUnmapMemory(device, uniformBufferMemory);
  vkFreeMemory(device, uniformBufferMemory, nullptr);
  vkDestroyBuffer(device, uniformBuffer, nullptr);

  vkDestroyDescriptorPool(device, descriptorPool, nullptr);

  vkFreeMemory(device, vertexBufferMemory, nullptr);
  vkDestroyBuffer(device, vertexBuffer, nullptr);
}

// Bind the vertices
void skybox::bindVertices() {
  verticesLoaded = engine::getInstance()->bindVertices(vertices, vertexBuffer, vertexBufferMemory);
}

// Bind the uniform descriptors
void skybox::bindDescriptors() {
  VkDevice device = engine::getInstance()->getDevice();

  VkDeviceSize bufferSize = sizeof(skyboxUniforms);
  engine::getInstance()->createVulkanBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffer, uniformBufferMemory);
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

	// Use the skybox shader prog
  shaderProgram* prog = (shaderProgram*)(child_resources["shaderProgs"]["skybox"]);
  VkCommandBuffer activeBuffer = engine::getInstance()->getActiveCommandBuffer();
  vkCmdBindPipeline(activeBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, prog->getPipeline());
  vkCmdBindDescriptorSets(activeBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, prog->getPipelineLayout(), 0, 1, &descriptorSet, 0, nullptr);
  
  // Move the view matrices to the GPU
  camera* activeCamera = scene::getActiveScene()->getActiveCamera();
  glm::mat4 viewMatrix = glm::mat4(glm::mat3(activeCamera->getViewMatrix()));
  glm::mat4 vpMatrix = activeCamera->getProjectionMatrix() * viewMatrix;

  // Get uniforms
  skyboxUniforms uniforms = {};
  uniforms.vpMatrix = vpMatrix;

  // Send uniforms to GPU
  memcpy(mapping, &uniforms, sizeof(uniforms));
  //engine::getInstance()->bindUniforms(uniforms, uniformBuffer, uniformBufferMemory);
  
  // Give draw commands
  VkBuffer vertexBuffers[] = {vertexBuffer};
  VkDeviceSize offsets[] = {0};
  vkCmdBindVertexBuffers(activeBuffer, 0, 1, vertexBuffers, offsets);
  vkCmdDraw(activeBuffer, vertices.size(), 1, 0, 0);
}
