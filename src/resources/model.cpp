#include "log.hpp"
#include "model.hpp"
#include "engine.hpp"

using namespace std;

// Sets up the vao
model::model(const std::string& name) : raw_resource(name) {
  numVertices = 0;
  verticesLoaded = false;
  descriptorsLoaded = false;
}

// Removes all vbos and the vao
model::~model() {
  VkDevice device = engine::getInstance()->getDevice();

  vkFreeMemory(device, uniformBufferMemory, nullptr);
  vkDestroyBuffer(device, uniformBuffer, nullptr);

  vkDestroyDescriptorPool(device, descriptorPool, nullptr);

  vkFreeMemory(device, vertexBufferMemory, nullptr);
  vkDestroyBuffer(device, vertexBuffer, nullptr);
}

// Adds a material
void model::addMaterial (const material& mat) {
  materials.push_back(mat);
}

// Creates a texture for the GPU
void model::setTexture(texture* tex) {
  child_resources["textures"][tex->getName()] = tex;
}

// Sends vertex data to the GPU
void model::setVertices(const vector<glm::vec3>& vertexList) {
  vertices.resize(vertexList.size());
  for (int i = 0; i < vertexList.size(); i++) {
    vertices[i].position.x = vertexList[i].x;
    vertices[i].position.y = vertexList[i].y;
    vertices[i].position.z = vertexList[i].z;
    vertices[i].position.w = 1.0f;;
  }
  numVertices = vertexList.size();
}

// Finalize mapping of vertices to UV coordinates
void model::setUVMapping(const vector<glm::vec2>& uvList) {
  if (uvList.size() > 0) {
    if (numVertices != uvList.size()) {
      recordLog("ERROR: Number of vertices does not equal number of uv coordinates");
    }

    for (int i = 0; i < uvList.size(); i++) {
      vertices[i].vertexUV = uvList[i];
    }
  }
}

// Sends normal data to the GPU
void model::setNormals(const vector<glm::vec3>& normalList) {
  if (numVertices != normalList.size()) {
    recordLog("ERROR: Number of vertices does not equal number of normal vectors");
  }

  for (int i = 0; i < normalList.size(); i++) {
    vertices[i].normal.x = normalList[i].x;
    vertices[i].normal.y = normalList[i].y;
    vertices[i].normal.z = normalList[i].z;
    vertices[i].normal.w = 1.0f;
  }
}

// Bind the uniform descriptors
void model::bindDescriptors() {
  VkDevice device = engine::getInstance()->getDevice();

  VkDeviceSize bufferSize = sizeof(modelUniforms);
  engine::getInstance()->createVulkanBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffer, uniformBufferMemory);

  // One day we will likely allow multiple textures, but for now, just use one.
  texture* tex = (texture*)(child_resources["textures"].begin()->second);
  shaderProgram* prog = (shaderProgram*)child_resources["shaderProgs"]["default"];
  descriptorsLoaded = prog->createVulkanDescriptorSet(descriptorPool, descriptorSet);

  VkDescriptorBufferInfo bufferInfo = {};
  bufferInfo.buffer = uniformBuffer;
  bufferInfo.offset = 0;
  bufferInfo.range = sizeof(modelUniforms);

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

// Send vertices to GPU
void model::bindVertices() {
  verticesLoaded = engine::getInstance()->bindVertices(vertices, vertexBuffer, vertexBufferMemory);
}

// Draws the model
void model::draw(modelUniforms uniforms) { 
  // Make sure that model vertices were sent to GPU
  if (!verticesLoaded) {
    bindVertices();
  }

  // Make sure that the uniform descriptors have been sent to the GPU
  if (!descriptorsLoaded) {
    bindDescriptors();
  }

  // Activate shader
  shaderProgram* prog = (shaderProgram*)child_resources["shaderProgs"]["default"];
  VkCommandBuffer activeBuffer = engine::getInstance()->getActiveCommandBuffer();
  vkCmdBindPipeline(activeBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, prog->getPipeline());
  vkCmdBindDescriptorSets(activeBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, prog->getPipelineLayout(), 0, 1, &descriptorSet, 0, nullptr);

  // Send uniforms to GPU
  engine::getInstance()->bindUniforms(uniforms, uniformBuffer, uniformBufferMemory);

  // Load textures into GPU
  int index = 0;
  for(auto p : child_resources["textures"]) {
    texture* tex = (texture*)(p.second);
    index++;
  }

  // Give draw commands
  VkBuffer vertexBuffers[] = {vertexBuffer};
  VkDeviceSize offsets[] = {0};
  vkCmdBindVertexBuffers(activeBuffer, 0, 1, vertexBuffers, offsets);
  vkCmdDraw(activeBuffer, vertices.size(), 1, 0, 0);
}

vector<glm::vec3> model::getVertexPositions() const {
  vector<glm::vec3> positions;

  for (auto v : vertices) {
    positions.push_back(v.position);
  }

  return positions;
}
