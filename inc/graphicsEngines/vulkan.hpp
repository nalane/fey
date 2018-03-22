#pragma once

/*
 * Rendering engine for Vulkan
 */

#include <vector>
#include <string>

#include "graphics.hpp"

class vulkan : public graphics {
private:
    // Data structures for Vulkan initialization
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

    // Static elements
    static const std::vector<const char*> requiredExtensions;
    static const std::vector<const char*> validationLayers;

    // Rendering
    QueueFamilyIndices queueIndices;
    SwapChainSupportDetails details;

    VkInstance vulkanInstance = VK_NULL_HANDLE;
    VkDebugReportCallbackEXT callback = VK_NULL_HANDLE;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device = VK_NULL_HANDLE;
    VkQueue graphicsQueue = VK_NULL_HANDLE;
    VkQueue presentQueue = VK_NULL_HANDLE;
    VkSurfaceKHR surface = VK_NULL_HANDLE;
    VkSwapchainKHR swapChain = VK_NULL_HANDLE;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    VkRenderPass renderPass = VK_NULL_HANDLE;
    VkCommandPool commandPool = VK_NULL_HANDLE;
    VkSemaphore imageAvailable = VK_NULL_HANDLE;
    VkSemaphore renderFinished = VK_NULL_HANDLE;
    VkImage depthImage = VK_NULL_HANDLE;
    VkDeviceMemory depthImageMemory = VK_NULL_HANDLE;
    VkImageView depthImageView = VK_NULL_HANDLE;

    // Buffers
    int activeBufferIndex = 0;
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

    virtual void glfwHints() {
        // Force use of Vulkan
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    }

    virtual bool initialize(bool fullscreen, unsigned int windowWidth, unsigned int windowHeight, const std::string& windowTitle, bool hideCursor, int numAASamples) {
        return initGLFW(fullscreen, windowWidth, windowHeight, windowTitle, hideCursor) && 
            initVulkan();
    }

    // Memory buffer interaction
    bool findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, uint32_t* output);
    bool copyVulkanBuffer(VkBuffer src, VkBuffer dst, VkDeviceSize size);

    // Depth buffer helper functions
    bool findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features, VkFormat& format);
    bool findDepthFormat(VkFormat& format);
    bool hasStencilComponent(VkFormat format) {
        return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
    }

    void cleanupSwapChain();

public:
    vulkan() : graphics(VULKAN) { }
    ~vulkan();

    bool recreateSwapChain();
    bool enableDepthBuffer();

    VkDevice getDevice() const { return device; }
    VkExtent2D getExtent() const { return swapChainExtent; }
    VkRenderPass getRenderPass() const { return renderPass; }
    VkCommandPool getCommandPool() const { return commandPool; }
    VkCommandBuffer getActiveCommandBuffer() const { return commandBuffers[activeBufferIndex]; }
    VkFramebuffer getActiveFramebuffer() const { return swapChainFramebuffers[activeBufferIndex]; }
    std::vector<VkCommandBuffer> getCommandBuffers() const { return commandBuffers; }

    bool programShouldRun() {return !glfwWindowShouldClose(window);}
    VkResult idle() {return vkDeviceWaitIdle(device);}

    // Virtual methods from graphics
    virtual void draw();
    virtual void resizeCallback(int width, int height) {
        recreateSwapChain();
    }

    // Create buffers for the shaders
    template <typename T>
    bool bindVertices(std::vector<T> vertices, VkBuffer& vertexBuffer, VkDeviceMemory& vertexBufferMemory);
    bool createVulkanBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);

    // Executing single time commands
    VkCommandBuffer beginSingleTimeCommands();
    void endSingleTimeCommands(VkCommandBuffer commandBuffer);

    // Create and manipulate images
    bool createImageView(VkImage image, VkImageViewType viewType, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t numLayers, VkImageView& imageView);
    bool createVulkanImage(uint32_t width, uint32_t height, uint32_t layers,
        VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags,
        VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
    void copyBufferToImage(VkBuffer buffer, VkImage image, std::vector<VkBufferImageCopy> bufferCopies);
    void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
};

// Send vertices to GPU
template <typename T>
bool vulkan::bindVertices(std::vector<T> vertices, VkBuffer& vertexBuffer, VkDeviceMemory& vertexBufferMemory) {
    VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    bool bufferCreated = createVulkanBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        stagingBuffer, stagingBufferMemory);
    if (!bufferCreated) {
        return false;
    }

    void* data;
    vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, vertices.data(), bufferSize);
    vkUnmapMemory(device, stagingBufferMemory);

    bufferCreated = createVulkanBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        vertexBuffer, vertexBufferMemory);
    if (!bufferCreated) {
        return false;
    }

    copyVulkanBuffer(stagingBuffer, vertexBuffer, bufferSize);

    vkDestroyBuffer(device, stagingBuffer, nullptr);
    vkFreeMemory(device, stagingBufferMemory, nullptr);
  
    return true;
}