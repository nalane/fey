#include "log.hpp"
#include "model.hpp"
#include "graphics.hpp"

using namespace std;

// Sets up the vao
model::model(const std::string& name) : raw_resource(name) {
  numVertices = 0;
  verticesLoaded = false;
  descriptorsLoaded = false;
}

// Removes all vbos and the vao
model::~model() {
  VkDevice device = graphics::getInstance()->getDevice();

  vkUnmapMemory(device, uniformBufferMemory);
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

// Gather vertex data
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

// Gather normals
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
  VkDevice device = graphics::getInstance()->getDevice();

  VkDeviceSize bufferSize = sizeof(modelUniforms);
  graphics::getInstance()->createVulkanBuffer(bufferSize,
    VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
    uniformBuffer, uniformBufferMemory);
  vkMapMemory(device, uniformBufferMemory, 0, sizeof(modelUniforms), 0, &mapping);

  shaderProgram* prog = (shaderProgram*)child_resources["shaderProgs"]["default"];
  descriptorsLoaded = prog->createVulkanDescriptorSet(descriptorPool, descriptorSet);

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
    texture* tex = (texture*)(child_resources["textures"].begin()->second);
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

// Send vertices to GPU
void model::bindVertices() {
  verticesLoaded = graphics::getInstance()->bindVertices(vertices, vertexBuffer, vertexBufferMemory);
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
  VkCommandBuffer activeBuffer = graphics::getInstance()->getActiveCommandBuffer();
  vkCmdBindPipeline(activeBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, prog->getPipeline());
  vkCmdBindDescriptorSets(activeBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, prog->getPipelineLayout(), 0, 1, &descriptorSet, 0, nullptr);

  // Send uniforms to GPU
  memcpy(mapping, &uniforms, sizeof(uniforms));

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
