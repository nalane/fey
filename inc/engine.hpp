#pragma once

/*
 * Contains functions for initializing and running the game.
 */

#include <string>
#include <vector>

#include "glHeaders.hpp"
#include "scene.hpp"

class engine {
private:
  // Static elements
  static engine* instance;
  static const std::vector<const char*> requiredExtensions;
  static const std::vector<const char*> validationLayers;

  // Nested data structures for Vulkan initialization
  struct QueueFamilyIndices {
    int graphicsFamily = -1;
    int presentFamily = -1;

    bool isComplete() {
      return graphicsFamily >= 0 && presentFamily >= 0;
    }
  };

  struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
  };

  // Values from config file
  unsigned int windowWidth;
  unsigned int windowHeight;
  int numAASamples;
  bool hideCursor;
  bool fullscreen;
  std::string windowTitle;
  std::string vertexShader;
  std::string fragmentShader;

  // Rendering
  GLFWwindow* window;
  QueueFamilyIndices queueIndices;
  SwapChainSupportDetails details;

  VkInstance vulkanInstance;
  VkDebugReportCallbackEXT callback;
  VkPhysicalDevice physicalDevice;
  VkDevice device;
  VkQueue graphicsQueue;
  VkQueue presentQueue;
  VkSurfaceKHR surface;
  VkSwapchainKHR swapChain;
  VkFormat swapChainImageFormat;
  VkExtent2D swapChainExtent;
  VkRenderPass renderPass;
  VkCommandPool commandPool;
  VkSemaphore imageAvailable;
  VkSemaphore renderFinished;

  int activeBufferIndex;
  std::vector<VkImage> swapChainImages;
  std::vector<VkImageView> swapChainImageViews;
  std::vector<VkFramebuffer> swapChainFramebuffers;
  std::vector<VkCommandBuffer> commandBuffers;

  // Functions to init Vulkan
  bool checkVulkanValidationLayerSupport();
  bool initVulkanInstance();
  bool initVulkanDebug();
  bool initVulkanSurface();
  QueueFamilyIndices findQueueFamilies(const VkPhysicalDevice& pDevice);
  SwapChainSupportDetails querySwapChainSupport(const VkPhysicalDevice& pDevice);
  bool isDeviceSuitable(const VkPhysicalDevice& pDevice);
  bool initVulkanDevice();
  bool initVulkanQueues();
  VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats);
  VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& presentModes);
  VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
  bool initVulkanSwapChain();
  bool initVulkanImageViews();
  bool initVulkanRenderPass();
  bool initVulkanFramebuffers();
  bool initVulkanCommandPool();
  bool initVulkanCommandBuffers();
  bool initVulkanSemaphores();
  bool initVulkan();

  bool findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, uint32_t* output);
  bool copyVulkanBuffer(VkBuffer src, VkBuffer dst, VkDeviceSize size);

  void cleanupSwapChain();

  bool initGLFW();
  bool initGraphics();
  bool initGame();
  void draw();

public:
  engine(const std::string& configFile);
  ~engine();

  static void createInstance(const std::string& configFile);
  static engine* getInstance();
  static void endInstance();

  bool recreateSwapChain();

  VkDevice getDevice() {return device;}
  VkExtent2D getExtent() {return swapChainExtent;}
  VkRenderPass getRenderPass() {return renderPass;}
  VkCommandBuffer getActiveCommandBuffer() {return commandBuffers[activeBufferIndex];}
  std::vector<VkCommandBuffer> getCommandBuffers() {return commandBuffers;}

  template <typename T>
  bool bindVertices(std::vector<T> vertices, VkBuffer& vertexBuffer, VkDeviceMemory& vertexBufferMemory);
  template <typename T>
  bool bindUniforms(T uniforms, VkBuffer& uniformBuffer, VkDeviceMemory& uniformBufferMemory);
  bool createVulkanBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);

  // Executing single time commands
  VkCommandBuffer beginSingleTimeCommands();
  void endSingleTimeCommands(VkCommandBuffer commandBuffer);

  bool createImageView(VkImage image, VkImageViewType viewType, VkFormat format, VkImageView& imageView);
  bool createVulkanImage(uint32_t width, uint32_t height, uint32_t layers, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
  void copyBufferToImage(VkBuffer buffer, VkImage image, std::vector<VkBufferImageCopy> bufferCopies);
  void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

  void runGame();
};

// Send vertices to GPU
template <typename T>
bool engine::bindVertices(std::vector<T> vertices, VkBuffer& vertexBuffer, VkDeviceMemory& vertexBufferMemory) {
  VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

  VkBuffer stagingBuffer;
  VkDeviceMemory stagingBufferMemory;
  bool bufferCreated = createVulkanBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);
  if (!bufferCreated) {
    return false;
  }

  void* data;
  vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
  memcpy(data, vertices.data(), bufferSize);
  vkUnmapMemory(device, stagingBufferMemory);

  bufferCreated = createVulkanBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);
  if (!bufferCreated) {
    return false;
  }

  copyVulkanBuffer(stagingBuffer, vertexBuffer, bufferSize);

  vkDestroyBuffer(device, stagingBuffer, nullptr);
  vkFreeMemory(device, stagingBufferMemory, nullptr);
  
  return true;
}

template <typename T>
bool engine::bindUniforms(T uniforms, VkBuffer& uniformBuffer, VkDeviceMemory& uniformBufferMemory) {
  VkDeviceSize bufferSize = sizeof(T);
  createVulkanBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffer, uniformBufferMemory);

  void* data;
  vkMapMemory(device, uniformBufferMemory, 0, sizeof(uniforms), 0, &data);
  memcpy(data, &uniforms, sizeof(uniforms));
  vkUnmapMemory(device, uniformBufferMemory);

  return true;
}