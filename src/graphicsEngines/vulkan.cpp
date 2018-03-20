#include <iostream>
#include <set>

#include "vulkan.hpp"
#include "log.hpp"
#include "engine.hpp"

using namespace std;

// Static constants
#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

const vector<const char*> vulkan::validationLayers = {
  "VK_LAYER_LUNARG_standard_validation"
};
const vector<const char*> vulkan::requiredExtensions = {
  VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

// Validation layer callback
static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugReportFlagsEXT flags,
        VkDebugReportObjectTypeEXT objType,
        uint64_t obj,
        size_t location,
        int32_t code,
        const char* layerPrefix,
        const char* msg,
        void* userData) {
    cerr << "Validation Layer Prefix: " << layerPrefix << endl;
    cerr << "Validation Layer Message:" << msg << endl; 

  return VK_FALSE;
}

vulkan::~vulkan() {
  cleanupSwapChain();

  vkDestroySemaphore(device, renderFinished, nullptr);
  vkDestroySemaphore(device, imageAvailable, nullptr);  

  vkDestroyCommandPool(device, commandPool, nullptr);
  vkDestroyDevice(device, nullptr);

  auto vkDestroyDebugReportCallbackEXT = (PFN_vkDestroyDebugReportCallbackEXT) vkGetInstanceProcAddr(vulkanInstance, "vkDestroyDebugReportCallbackEXT");
  if (vkDestroyDebugReportCallbackEXT != nullptr) {
      vkDestroyDebugReportCallbackEXT(vulkanInstance, callback, nullptr);
  }
  vkDestroySurfaceKHR(vulkanInstance, surface, nullptr);
  vkDestroyInstance(vulkanInstance, nullptr);
}

// Make sure needed validation layers are supported
bool vulkan::checkVulkanValidationLayerSupport() {
  // Identify validation layers
  uint32_t layerCount;
  vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
  vector<VkLayerProperties> availableLayers(layerCount);
  vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

  // Transform required layers into a set that we can remove available layers from
  set<string> requiredLayers(validationLayers.begin(), validationLayers.end());
  for (const auto& layer : availableLayers) {
    requiredLayers.erase(layer.layerName);
    if (requiredLayers.empty()) {
      break;
    }
  }
  if (!requiredLayers.empty()) {
    recordLog("ERROR: Not all required validation layers are available");
    return false;
  }

  return true;
}

// Begin Vulkan
bool vulkan::initVulkanInstance() {
  // Enable validation layers, if needed
  if (enableValidationLayers && !checkVulkanValidationLayerSupport()) {
    recordLog("FATAL ERROR: Validation layers were requested but could not be generated");
    return false;
  }

  // Optional struct, but may help Vulkan optimize
  VkApplicationInfo appInfo = {};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = engine::getInstance()->getWindowTitle().c_str();
  appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
  appInfo.pEngineName = "Fey";
  appInfo.engineVersion = VK_MAKE_VERSION(0, 0, 1);
  appInfo.apiVersion = VK_API_VERSION_1_0;

  // Get required extensions
  uint32_t requiredExtensionCount = 0;
  const char** requiredExtensionsArr;
  requiredExtensionsArr = glfwGetRequiredInstanceExtensions(&requiredExtensionCount);
  vector<const char*> rExtensions(requiredExtensionsArr, requiredExtensionsArr + requiredExtensionCount);
  if (enableValidationLayers) {
    rExtensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
  }

  // Get available extensions
  uint32_t availableExtensionCount = 0;
  vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionCount, nullptr);
  vector<VkExtensionProperties> availableExtensions(availableExtensionCount);
  vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionCount, availableExtensions.data());

  // Make a set of all available extensions to compare against
  set<string> availableExtensionsSet;
  for (VkExtensionProperties ext : availableExtensions) {
    availableExtensionsSet.insert(ext.extensionName);
  }

  // Make sure all required extensions are available
  for (const char* ext : rExtensions) {
    if (availableExtensionsSet.find(ext) == availableExtensionsSet.end()) {
      recordLog("FATAL ERROR: Not all required extensions are available");
      return false;
    }
  }

  // Struct containing info for creating instance
  VkInstanceCreateInfo createInfo = {};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;
  createInfo.enabledExtensionCount = rExtensions.size();
  createInfo.ppEnabledExtensionNames = rExtensions.data();
  if (enableValidationLayers) {
    createInfo.enabledLayerCount = validationLayers.size();
    createInfo.ppEnabledLayerNames = validationLayers.data();
  } else {
    createInfo.enabledLayerCount = 0;
  }

  // Create instance
  VkResult result = vkCreateInstance(&createInfo, nullptr, &vulkanInstance);
  if (result != VK_SUCCESS) {
    recordLog("FATAL ERROR: Unable to create Vulkan instance!");
    return false;
  }

  return true;
}

bool vulkan::initVulkanDebug() {
  if (!enableValidationLayers)
    return true;

  // Load debug report callback function
  auto vkCreateDebugReportCallbackEXT = (PFN_vkCreateDebugReportCallbackEXT) vkGetInstanceProcAddr(vulkanInstance, "vkCreateDebugReportCallbackEXT");
  if (vkCreateDebugReportCallbackEXT == nullptr) {
    recordLog("FATAL ERROR: Debugging requested, but no debug support is provided on this system.");
    return false;
  }

  VkDebugReportCallbackCreateInfoEXT createInfo = {};
  createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
  createInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
  createInfo.pfnCallback = debugCallback;

  VkResult result = vkCreateDebugReportCallbackEXT(vulkanInstance, &createInfo, nullptr, &callback);
  if (result != VK_SUCCESS) {
    recordLog("FATAL ERROR: Debugging was requested, but the callback could not be initiated");
    return false;
  }

  return true;
}

bool vulkan::initVulkanSurface() {
  VkResult result = glfwCreateWindowSurface(vulkanInstance, window, nullptr, &surface);
  if (result != VK_SUCCESS) {
    recordLog("FATAL ERROR: Could not create window surface!");
    return false;
  }

  return true;
}

vulkan::QueueFamilyIndices vulkan::findQueueFamilies(const VkPhysicalDevice& pDevice) {
  // Get number of queue families
  uint32_t numQueueFamilies = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(pDevice, &numQueueFamilies, nullptr);

  // Get queue families
  vector<VkQueueFamilyProperties> queueFamilies(numQueueFamilies);
  vkGetPhysicalDeviceQueueFamilyProperties(pDevice, &numQueueFamilies, queueFamilies.data());

  // Find indices of all needed queues
  QueueFamilyIndices indices;
  for (int i = 0; i < numQueueFamilies; i++) {
    VkQueueFamilyProperties family = queueFamilies[i];
    if (family.queueCount <= 0) {
      continue;
    }

    // Find graphics queue
    if (family.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
      indices.graphicsFamily = i;
    }

    // Find present queue
    VkBool32 presentSupport = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(pDevice, i, surface, &presentSupport);
    if (presentSupport) {
      indices.presentFamily = i;
    }

    // All queues have been found
    if (indices.isComplete()) {
      break;
    }
  }

  return indices;
}

vulkan::SwapChainSupportDetails vulkan::querySwapChainSupport(const VkPhysicalDevice& pDevice) {
  SwapChainSupportDetails supportDetails;

  // Check for the device's capabilities
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(pDevice, surface, &supportDetails.capabilities);

  // See what formats the device supports
  uint32_t formatCount;
  vkGetPhysicalDeviceSurfaceFormatsKHR(pDevice, surface, &formatCount, nullptr);
  if (formatCount != 0) {
    supportDetails.formats.resize(formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(pDevice, surface, &formatCount, supportDetails.formats.data());
  }

  // See how many present modes there are
  uint32_t presentModeCount;
  vkGetPhysicalDeviceSurfacePresentModesKHR(pDevice, surface, &presentModeCount, nullptr);
  if (presentModeCount != 0) {
    supportDetails.presentModes.resize(presentModeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(pDevice, surface, &presentModeCount, supportDetails.presentModes.data());
  }

  return supportDetails;
}

bool vulkan::isDeviceSuitable(const VkPhysicalDevice& pDevice) {
  // Make sure device has the right queues
  QueueFamilyIndices indices = findQueueFamilies(pDevice);
  if (!indices.isComplete()) {
    return false;
  }

  // Get extensions supported by the device
  uint32_t numAvailableExtensions;
  vkEnumerateDeviceExtensionProperties(pDevice, nullptr, &numAvailableExtensions, nullptr);
  vector<VkExtensionProperties> availableExtensions(numAvailableExtensions);
  vkEnumerateDeviceExtensionProperties(pDevice, nullptr, &numAvailableExtensions, availableExtensions.data());

  // Make sure device supports needed extensions
  set<string> rExtensions(requiredExtensions.begin(), requiredExtensions.end());
  for (const auto& ext : availableExtensions) {
    rExtensions.erase(ext.extensionName);
    if (rExtensions.empty()) {
      break;
    }
  }
  if (!rExtensions.empty()) {
    return false;
  }

  // Make sure swap chain support is adequate
  SwapChainSupportDetails supportDetails = querySwapChainSupport(pDevice);
  bool swapChainAdequate = !supportDetails.formats.empty() && !supportDetails.presentModes.empty();
  if (!swapChainAdequate) {
    return false;
  }

  // Get device properties and features
  VkPhysicalDeviceProperties props;
  VkPhysicalDeviceFeatures features;
  vkGetPhysicalDeviceProperties(pDevice, &props);
  vkGetPhysicalDeviceFeatures(pDevice, &features);

  // Determine if device has needed properties and features
  // This list will expand as time goes on
  if (features.shaderFloat64 && features.samplerAnisotropy) {
    queueIndices = indices;
    details = supportDetails;
    return true;
  }

  return false;
}

bool vulkan::initVulkanDevice() {
  // Get the number of graphics cards available
  uint32_t deviceCount = 0;
  vkEnumeratePhysicalDevices(vulkanInstance, &deviceCount, nullptr);
  if (deviceCount == 0) {
    recordLog("FATAL ERROR: No GPUs with Vulkan support");
  }

  // Get the graphics card to use
  vector<VkPhysicalDevice> devices(deviceCount);
  vkEnumeratePhysicalDevices(vulkanInstance, &deviceCount, devices.data());
  for (const auto& d : devices) {
    if (isDeviceSuitable(d)) {
      physicalDevice = d;
      break;
    }
  }
  if (physicalDevice == VK_NULL_HANDLE) {
    recordLog("FATAL ERROR: No suitable GPU found");
    return false;
  }

  // Create queue creation info for all needed queues
  set<int> uniqueQueueFamilies = {queueIndices.graphicsFamily, queueIndices.presentFamily};
  float queuePriority = 1.0f;
  vector<VkDeviceQueueCreateInfo> queueCreateInfos;
  for (int queueFamily : uniqueQueueFamilies) {
    VkDeviceQueueCreateInfo queueCreateInfo = {};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = queueFamily;
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = &queuePriority;
    queueCreateInfos.push_back(queueCreateInfo);
  }

  // Create logical device
  queuePriority = 1.0f;
  VkDeviceQueueCreateInfo queueCreateInfo = {};
  queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
  queueCreateInfo.queueFamilyIndex = queueIndices.graphicsFamily;
  queueCreateInfo.queueCount = 1;
  queueCreateInfo.pQueuePriorities = &queuePriority;

  VkPhysicalDeviceFeatures deviceFeatures = {};
  deviceFeatures.samplerAnisotropy = VK_TRUE;

  VkDeviceCreateInfo createInfo = {};
  createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  createInfo.pQueueCreateInfos = &queueCreateInfo;
  createInfo.queueCreateInfoCount = 1;
  createInfo.pEnabledFeatures = &deviceFeatures;
  createInfo.enabledExtensionCount = requiredExtensions.size();
  createInfo.ppEnabledExtensionNames = requiredExtensions.data();
  createInfo.enabledLayerCount = 0;
  createInfo.queueCreateInfoCount = queueCreateInfos.size();
  createInfo.pQueueCreateInfos = queueCreateInfos.data();

  VkResult result = vkCreateDevice(physicalDevice, &createInfo, nullptr, &device);
  if (result != VK_SUCCESS) {
    recordLog("FATAL ERROR: Could not create logical device");
    return false;
  }

  return true;
}

bool vulkan::initVulkanQueues() {
  vkGetDeviceQueue(device, queueIndices.graphicsFamily, 0, &graphicsQueue);
  vkGetDeviceQueue(device, queueIndices.presentFamily, 0, &presentQueue);

  return true;
}

VkSurfaceFormatKHR vulkan::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats) {
  if (formats.size() == 1 && formats[0].format == VK_FORMAT_UNDEFINED) {
    return {VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
  }

  for (const auto& format : formats) {
    if (format.format == VK_FORMAT_B8G8R8A8_UNORM && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
      return format;
    }
  }

  return formats[0];
}

VkPresentModeKHR vulkan::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& presentModes) {
  VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_KHR;

  for (const auto& mode : presentModes) {
    if (mode == VK_PRESENT_MODE_MAILBOX_KHR) {
      return mode;
    } else if (mode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
      bestMode = mode;
    }
  }

  return bestMode;
}

VkExtent2D vulkan::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
  if (capabilities.currentExtent.width != numeric_limits<uint32_t>::max()) {
    return capabilities.currentExtent;
  }

  int width, height;
  glfwGetWindowSize(window, &width, &height);

  VkExtent2D actualExtent = {static_cast<uint32_t>(width),
    static_cast<uint32_t>(height)};
  return actualExtent;
}

bool vulkan::initVulkanSwapChain() {
  VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(details.formats);
  VkPresentModeKHR presentMode = chooseSwapPresentMode(details.presentModes);
  VkExtent2D extent = chooseSwapExtent(details.capabilities);

  // Determine number of images in swap chain
  uint32_t imageCount = details.capabilities.minImageCount + 1;
  if (details.capabilities.maxImageCount > 0 && imageCount > details.capabilities.maxImageCount) {
    imageCount = details.capabilities.maxImageCount;
  }

  // Struct to create swap chain
  uint32_t indices[] = { static_cast<uint32_t>(queueIndices.graphicsFamily),
    static_cast<uint32_t>(queueIndices.presentFamily) };
  VkSwapchainCreateInfoKHR createInfo = {};
  createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  createInfo.surface = surface;
  createInfo.presentMode = presentMode;
  createInfo.clipped = VK_TRUE;
  createInfo.oldSwapchain = VK_NULL_HANDLE;
  createInfo.preTransform = details.capabilities.currentTransform;
  createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  createInfo.minImageCount = imageCount;
  createInfo.imageFormat = surfaceFormat.format;
  createInfo.imageColorSpace = surfaceFormat.colorSpace;
  createInfo.imageExtent = extent;
  createInfo.imageArrayLayers = 1;
  createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
  if (queueIndices.graphicsFamily != queueIndices.presentFamily) {
    createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    createInfo.queueFamilyIndexCount = 2;
    createInfo.pQueueFamilyIndices = indices;
  } else {
    createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    createInfo.queueFamilyIndexCount = 0;
    createInfo.pQueueFamilyIndices = nullptr;
  }

  // Create the actual swap chain
  VkResult result = vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapChain);
  if (result != VK_SUCCESS) {
    recordLog("FATAL ERROR: Could not create swap chain");
    return false;
  }

  // Get swapchain images
  vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);
  swapChainImages.resize(imageCount);
  vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data());

  // Store image format and extents for future use
  swapChainImageFormat = surfaceFormat.format;
  swapChainExtent = extent;

  return true;
}

bool vulkan::initVulkanImageViews() {
  swapChainImageViews.resize(swapChainImages.size());

  for (int i = 0; i < swapChainImages.size(); i++) {
    VkImageView imageView;
    createImageView(swapChainImages[i], VK_IMAGE_VIEW_TYPE_2D, swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1, imageView);
    swapChainImageViews[i] = imageView;
  }

  return true;
}

bool vulkan::initVulkanRenderPass() {
  // Describes our color attachment
  VkAttachmentDescription colorAttachment = {};
  colorAttachment.format = swapChainImageFormat;
  colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
  colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

  VkAttachmentReference colorAttachmentRef = {};
  colorAttachmentRef.attachment = 0;
  colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  // Set up depth buffering
  VkFormat format;
  findDepthFormat(format);
  VkAttachmentDescription depthAttachment = {};
  depthAttachment.format = format;
  depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
  depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

  VkAttachmentReference depthAttachmentRef = {};
  depthAttachmentRef.attachment = 1;
  depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

  // Generate subpass
  VkSubpassDescription subpass = {};
  subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
  subpass.colorAttachmentCount = 1;
  subpass.pColorAttachments = &colorAttachmentRef;
  subpass.pDepthStencilAttachment = &depthAttachmentRef;

  VkSubpassDependency dependency = {};
  dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
  dependency.dstSubpass = 0;
  dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  dependency.srcAccessMask = 0;
  dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

  // Render pass creation info
  vector<VkAttachmentDescription> attachments = {colorAttachment, depthAttachment};
  VkRenderPassCreateInfo renderPassInfo = {};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  renderPassInfo.attachmentCount = attachments.size();
  renderPassInfo.pAttachments = attachments.data();
  renderPassInfo.subpassCount = 1;
  renderPassInfo.pSubpasses = &subpass;
  renderPassInfo.dependencyCount = 1;
  renderPassInfo.pDependencies = &dependency;

  // Create Render pass
  VkResult result = vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass);
  if (result != VK_SUCCESS) {
    recordLog("FATAL ERROR: Could not create render pass");
    return false;
  }

  return true;
}

bool vulkan::initVulkanFramebuffers() {
  swapChainFramebuffers.resize(swapChainImageViews.size());

  for (int i = 0; i < swapChainImageViews.size(); i++) {
    VkImageView view = swapChainImageViews[i];
    vector<VkImageView> attachments = {view, depthImageView};

    VkFramebufferCreateInfo framebufferInfo = {};
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass = renderPass;
    framebufferInfo.attachmentCount = attachments.size();
    framebufferInfo.pAttachments = attachments.data();
    framebufferInfo.width = swapChainExtent.width;
    framebufferInfo.height = swapChainExtent.height;
    framebufferInfo.layers = 1;

    VkResult result = vkCreateFramebuffer(device, &framebufferInfo, nullptr, &swapChainFramebuffers[i]);
    if (result != VK_SUCCESS) {
      recordLog("FATAL ERROR: Could not create framebuffer");
      return false;
    }
  }

  return true;
}

bool vulkan::initVulkanCommandPool() {
  VkCommandPoolCreateInfo poolInfo = {};
  poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  poolInfo.queueFamilyIndex = queueIndices.graphicsFamily;
  poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

  VkResult result = vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool);
  if (result != VK_SUCCESS) {
    recordLog("FATAL ERROR: Could not create Vulkan command pool");
    return false;
  }

  return true;
}

bool vulkan::initVulkanCommandBuffers() {
  commandBuffers.resize(swapChainFramebuffers.size());

  VkCommandBufferAllocateInfo allocInfo = {};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.commandPool = commandPool;
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandBufferCount = commandBuffers.size();

  VkResult result = vkAllocateCommandBuffers(device, &allocInfo, commandBuffers.data());
  if (result != VK_SUCCESS) {
    recordLog("FATAL ERROR: Could not allocate command buffers!");
    return false;
  }

  return true;
}

bool vulkan::initVulkanSemaphores() {
  VkSemaphoreCreateInfo semaphoreInfo = {};
  semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

  VkResult imageResult = vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailable);
  VkResult renderResult = vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinished);

  if (imageResult != VK_SUCCESS || renderResult != VK_SUCCESS) {
    recordLog("FATAL ERROR: Could not create semaphores");
    return false;
  }

  return true;
}

bool vulkan::initVulkan() {
  if (!initVulkanInstance())
    return false;

  if (!initVulkanDebug())
    return false;

  if (!initVulkanSurface())
    return false;

  if (!initVulkanDevice())
    return false;

  if (!initVulkanQueues())
    return false;

  if (!initVulkanSwapChain())
    return false;

  if (!initVulkanImageViews())
    return false;

  if (!initVulkanRenderPass())
    return false;

  if (!initVulkanCommandPool())
    return false;

  if (!enableDepthBuffer())
    return false;

  if (!initVulkanFramebuffers())
    return false;

  if (!initVulkanCommandBuffers()) 
    return false;

  if (!initVulkanSemaphores())
    return false;

  return true;
}

bool vulkan::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, uint32_t* output) {
  VkPhysicalDeviceMemoryProperties memProperties;
  vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

  for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
    if (typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
      *output =  i;
      return true;
    }
  }

  recordLog("ERROR: Could not find suitable memory type!");
  return false;
}

bool vulkan::copyVulkanBuffer(VkBuffer src, VkBuffer dst, VkDeviceSize size) {
  VkCommandBuffer commandBuffer = beginSingleTimeCommands();

  VkBufferCopy copyRegion = {};
  copyRegion.srcOffset = 0;
  copyRegion.dstOffset = 0;
  copyRegion.size = size;
  vkCmdCopyBuffer(commandBuffer, src, dst, 1, &copyRegion);

  endSingleTimeCommands(commandBuffer);

  return true;
}

bool vulkan::findSupportedFormat(const vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features, VkFormat& format) {
  for (VkFormat candidate : candidates) {
    VkFormatProperties props;
    vkGetPhysicalDeviceFormatProperties(physicalDevice, candidate, &props);

    if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
      format = candidate;
      return true;
    } else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
      format = candidate;
      return true;
    }
  }

  recordLog("ERROR: Could not find supported format!");
  return false;
}

bool vulkan::findDepthFormat(VkFormat& format) {
  bool result = findSupportedFormat(
    {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
    VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT, format);

  if (!result) {
    recordLog("FATAL ERROR: Could not find supported depth buffer format!");
    return false;
  }

  return true;
}

void vulkan::cleanupSwapChain() {
  for (auto buffer : swapChainFramebuffers) {
    vkDestroyFramebuffer(device, buffer, nullptr);
  }

  vkFreeCommandBuffers(device, commandPool, commandBuffers.size(), commandBuffers.data());

  resourceHandler::getInstance()->unloadShaders();
  vkDestroyRenderPass(device, renderPass, nullptr);

  vkDestroyImageView(device, depthImageView, nullptr);
  vkDestroyImage(device, depthImage, nullptr);
  vkFreeMemory(device, depthImageMemory, nullptr);

  for (auto view : swapChainImageViews) {
    vkDestroyImageView(device, view, nullptr);
  }

  vkDestroySwapchainKHR(device, swapChain, nullptr);
}

// Recreate swap chain if needed
bool vulkan::recreateSwapChain() {
  // Consider the situation where the window is minimized
  int width, height;
  glfwGetWindowSize(window, &width, &height);
  if (width == 0 || height == 0)
    return true;

  vkDeviceWaitIdle(device);

  cleanupSwapChain();
  if (!initVulkanSwapChain())
    return false;

  if (!initVulkanImageViews())
    return false;

  if (!enableDepthBuffer())
    return false;
  
  if (!initVulkanRenderPass())
    return false;

  resourceHandler::getInstance()->reloadShaders();

  if (!initVulkanFramebuffers())
    return false;

  if (!initVulkanCommandBuffers())
    return false;

  return true;
}

bool vulkan::createVulkanBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) {
  VkBufferCreateInfo bufferInfo = {};
  bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  bufferInfo.size = size;
  bufferInfo.usage = usage;
  bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  VkResult result = vkCreateBuffer(device, &bufferInfo, nullptr, &buffer);
  if (result != VK_SUCCESS) {
    recordLog("ERROR: Could not create buffer");
    return false;
  }

  VkMemoryRequirements memRequirements;
  vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

  uint32_t memoryIndex;
  findMemoryType(memRequirements.memoryTypeBits, properties, &memoryIndex);

  VkMemoryAllocateInfo allocInfo = {};
  allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  allocInfo.allocationSize = memRequirements.size;
  allocInfo.memoryTypeIndex = memoryIndex;

  result = vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory);
  if (result != VK_SUCCESS) {
    recordLog("ERROR: Failed to allocate buffer memory");
    return false;
  }
  vkBindBufferMemory(device, buffer, bufferMemory, 0);  

  return true;
}

VkCommandBuffer vulkan::beginSingleTimeCommands() {
  VkCommandBufferAllocateInfo allocInfo = {};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandPool = commandPool;
  allocInfo.commandBufferCount = 1;

  VkCommandBuffer commandBuffer;
  vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

  VkCommandBufferBeginInfo beginInfo = {};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

  vkBeginCommandBuffer(commandBuffer, &beginInfo);

  return commandBuffer;
}

void vulkan::endSingleTimeCommands(VkCommandBuffer commandBuffer) {
  vkEndCommandBuffer(commandBuffer);

  VkSubmitInfo submitInfo = {};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &commandBuffer;

  vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
  vkQueueWaitIdle(graphicsQueue);

  vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
}

bool vulkan::createImageView(VkImage image, VkImageViewType viewType, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t numLayers, VkImageView& imageView) {
  VkImageViewCreateInfo createInfo = {};
  createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
  createInfo.image = image;
  createInfo.viewType = viewType;
  createInfo.format = format;
  createInfo.subresourceRange.aspectMask = aspectFlags;
  createInfo.subresourceRange.baseMipLevel = 0;
  createInfo.subresourceRange.levelCount = 1;
  createInfo.subresourceRange.baseArrayLayer = 0;
  createInfo.subresourceRange.layerCount = numLayers;

  VkResult result = vkCreateImageView(device, &createInfo, nullptr, &imageView);
  if (result != VK_SUCCESS) {
    recordLog("FATAL ERROR: Could not create image views");
    return false;
  }

  return true;
}

bool vulkan::createVulkanImage(uint32_t width, uint32_t height, uint32_t layers, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory) {
  VkFormatProperties props;
  vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &props);

  // Image create info struct
  VkImageCreateInfo imageInfo = {};
  imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
  imageInfo.imageType = VK_IMAGE_TYPE_2D;
  imageInfo.extent.width = width;
  imageInfo.extent.height = height;
  imageInfo.extent.depth = 1;
  imageInfo.mipLevels = 1;
  imageInfo.arrayLayers = layers;
  imageInfo.format = format;
  imageInfo.tiling = tiling;
  imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  imageInfo.usage = usage;
  imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
  imageInfo.flags = flags;

  // Create image
  VkResult result = vkCreateImage(device, &imageInfo, nullptr, &image);
  if (result != VK_SUCCESS) {
    recordLog("ERROR: Failed to create image");
    return false;
  }

  // Get memory requirements for image
  VkMemoryRequirements memRequirements;
  vkGetImageMemoryRequirements(device, image, &memRequirements);

  // Get index of memory location to use
  uint32_t memoryIndex;
  bool memoryResult = findMemoryType(memRequirements.memoryTypeBits, properties, &memoryIndex);
  if (!memoryResult) {
    recordLog("ERROR: Couldn't find location to place texture");
    return false;
  }

  // Memory allocation info
  VkMemoryAllocateInfo allocInfo = {};
  allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  allocInfo.allocationSize = memRequirements.size;
  allocInfo.memoryTypeIndex = memoryIndex;

  // Allocate and bind memory
  result = vkAllocateMemory(device, &allocInfo, nullptr, &imageMemory);
  if (result != VK_SUCCESS) {
    recordLog("ERROR: Failed to allocate memory");
    return false;
  }
  vkBindImageMemory(device, image, imageMemory, 0);

  return true;
}

void vulkan::copyBufferToImage(VkBuffer buffer, VkImage image, vector<VkBufferImageCopy> bufferCopies) {
  VkCommandBuffer commandBuffer = beginSingleTimeCommands();
  vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, bufferCopies.data());
  endSingleTimeCommands(commandBuffer);
}

void vulkan::transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout) {
  VkCommandBuffer commandBuffer = beginSingleTimeCommands();

  VkPipelineStageFlags sourceStage;
  VkPipelineStageFlags destinationStage;

  VkImageMemoryBarrier barrier = {};
  barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
  barrier.oldLayout = oldLayout;
  barrier.newLayout = newLayout;
  barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barrier.image = image;
  barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  barrier.subresourceRange.baseMipLevel = 0;
  barrier.subresourceRange.levelCount = 1;
  barrier.subresourceRange.baseArrayLayer = 0;
  barrier.subresourceRange.layerCount = 1;

  if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
    barrier.srcAccessMask = 0;
    barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

    sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
  }

  else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

    sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
  }

  else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
    barrier.srcAccessMask = 0;
    barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

    sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
  }

  else {
    recordLog("ERROR: Unsupported layout transition");
    return;
  }

  if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

    if (hasStencilComponent(format)) {
      barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
    }
  } else {
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  }

  vkCmdPipelineBarrier(commandBuffer, sourceStage, destinationStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);

  endSingleTimeCommands(commandBuffer);
}

bool vulkan::enableDepthBuffer() {
  VkFormat format;
  if (!findDepthFormat(format)) {
    recordLog("FATAL ERROR: Depth buffering could not be initialized");
    return false;
  }

  createVulkanImage(swapChainExtent.width, swapChainExtent.height, 1, format, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, 0, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImage, depthImageMemory);
  createImageView(depthImage, VK_IMAGE_VIEW_TYPE_2D, format, VK_IMAGE_ASPECT_DEPTH_BIT, 1, depthImageView);
  transitionImageLayout(depthImage, format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
  return true;
}

void vulkan::draw() {
  double currentTime = glfwGetTime();
  
  VkCommandBufferBeginInfo beginInfo = {};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
  beginInfo.pInheritanceInfo = nullptr;

  vkBeginCommandBuffer(getActiveCommandBuffer(), &beginInfo);

  // Render pass create info
  vector<VkClearValue> clearValues(2);
  clearValues[0].color = {0.0f, 0.0f, 0.0f, 1.0f};
  clearValues[1].depthStencil = {1.0f, 0};
  VkRenderPassBeginInfo renderPassInfo = {};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  renderPassInfo.renderPass = renderPass;
  renderPassInfo.framebuffer = swapChainFramebuffers[activeBufferIndex];
  renderPassInfo.renderArea.offset = {0, 0};
  renderPassInfo.renderArea.extent = swapChainExtent;
  renderPassInfo.clearValueCount = clearValues.size();
  renderPassInfo.pClearValues = clearValues.data();

  // Execute render pass
  vkCmdBeginRenderPass(getActiveCommandBuffer(), &renderPassInfo, VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS);
  scene::getActiveScene()->draw();
  vkCmdEndRenderPass(getActiveCommandBuffer());

  // End command buffer
  VkResult result = vkEndCommandBuffer(getActiveCommandBuffer());
  if (result != VK_SUCCESS) {
    recordLog("FATAL ERROR: Failed to record command buffer!");
    return;
  }

  // Increment active buffer
  activeBufferIndex = (activeBufferIndex + 1) % commandBuffers.size();

  // Get next image to draw to
  uint32_t imageIndex;
  result = vkAcquireNextImageKHR(device, swapChain, numeric_limits<uint64_t>::max(), imageAvailable, VK_NULL_HANDLE, &imageIndex);
  if (result == VK_ERROR_OUT_OF_DATE_KHR) {
    recreateSwapChain();
    return;
  } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
    recordLog("FATAL ERROR: Failed to acquire swap chain image");
    return;
  }

  // Queue submission info struct
  VkSemaphore waitSemaphores[] = {imageAvailable};
  VkSemaphore signalSemaphores[] = {renderFinished};
  VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
  VkSubmitInfo submitInfo = {};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submitInfo.waitSemaphoreCount = 1;
  submitInfo.pWaitSemaphores = waitSemaphores;
  submitInfo.pWaitDstStageMask = waitStages;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &commandBuffers[imageIndex];
  submitInfo.signalSemaphoreCount = 1;
  submitInfo.pSignalSemaphores = signalSemaphores;

  // Submit command buffer to draw
  result = vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
  if (result != VK_SUCCESS) {
    recordLog("FATAL ERROR: Could not draw command buffer");
    return;
  }

  // Presentation info struct
  VkSwapchainKHR swapChains[] = {swapChain};
  VkPresentInfoKHR presentInfo = {};
  presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  presentInfo.waitSemaphoreCount = 1;
  presentInfo.pWaitSemaphores = signalSemaphores;
  presentInfo.swapchainCount = 1;
  presentInfo.pSwapchains = swapChains;
  presentInfo.pImageIndices = &imageIndex;
  presentInfo.pResults = nullptr;

  // Present frame
  result = vkQueuePresentKHR(presentQueue, &presentInfo);
  if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
    recreateSwapChain();
  } else if (result != VK_SUCCESS) {
    recordLog("FATAL ERROR: Failed to present swap chain image!");
    return;
  }

  // Idle until frame is drawn
  result = idle();
  if (result == VK_ERROR_DEVICE_LOST) {
    recordLog("FATAL ERROR: Device lost probably due to timeout");
    return;
  }
}