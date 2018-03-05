#include <math.h>
#include <time.h>

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <set>
#include <algorithm>

#include "engine.hpp"
#include "log.hpp"
#include "paths.hpp"
#include "main_scene.hpp"

using namespace std;

// Static members
engine* engine::instance;
#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif
const vector<const char*> engine::validationLayers = {
  "VK_LAYER_LUNARG_standard_validation"
};
const vector<const char*> engine::requiredExtensions = {
  VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

// GLFW and vulkan callbacks
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  scene::getActiveScene()->keyPress(key, action, mods);
}

static void cursor_callback(GLFWwindow* window, double xpos, double ypos) {
  scene::getActiveScene()->mousePosition(xpos, ypos);
}

static void window_resize_callback(GLFWwindow* window, int width, int height) {
  engine::getInstance()->recreateSwapChain();
}

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

// Load the engine. Uses the values found in configFile
engine::engine(const string& configFile) : device(VK_NULL_HANDLE), activeBufferIndex(0) {
  resourceHandler::createInstance();
  ifstream fin(configFile.c_str());
  if (fin.is_open()) {
    string dataPath, libraryPath;

    fin >> windowWidth;
    fin >> windowHeight;
    fin >> numAASamples;
    fin >> hideCursor;
    fin >> fullscreen;
    fin.ignore(1, '\n');
    getline(fin, windowTitle);
    getline(fin, dataPath);
    getline(fin, libraryPath);
    getline(fin, vertexShader);
    getline(fin, fragmentShader);

    fin.close();

    recordLog("Values found in config file:");
    recordLog("----------------------------");
    recordLog("Window Width: " + to_string(windowWidth));
    recordLog("Window Height: " + to_string(windowHeight));
    recordLog("Window Title: " + windowTitle);
    recordLog("Data Path: " + dataPath);
    recordLog("Library Path: " + libraryPath);
    recordLog("Vertex Shader: " + vertexShader);
    recordLog("Fragment Shader: " + fragmentShader);

    setDataFolder(dataPath);
    setLibraryFolder(libraryPath);
	
    vertexShader = getDataFolderPath(vertexShader);
    fragmentShader = getDataFolderPath(fragmentShader);
  }

  else {
    recordLog("FATAL ERROR: Could not open config file " + configFile);
  }
}

// Destroys the game and all pointers used
engine::~engine() {
  scene::endActiveScene();
  resourceHandler::endInstance();

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
  
  glfwDestroyWindow(window);
  glfwTerminate();
}

// Create an instance of the engine
void engine::createInstance(const string& configFile) {
  instance = new engine(configFile);
}

// Return the instance of the engine
engine* engine::getInstance() {
  return instance;
}

// Delete the instance
void engine::endInstance() {
  delete instance;
}

// GLFW Initialization
bool engine::initGLFW() {
  if (!glfwInit()) {
    recordLog("FATAL ERROR: Could not initialize GLFW!");
    return false;
  }

  // Force use of OpenGL core
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

  // Determine if we should do it fullscreen
  GLFWmonitor* monitor = nullptr;
  if (fullscreen) {
    monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* videoMode = glfwGetVideoMode(monitor);
    windowWidth = videoMode->width;
    windowHeight = videoMode->height;
  }

  // Create the window
  window = glfwCreateWindow(windowWidth, windowHeight, windowTitle.c_str(), monitor, nullptr);
  if (!window) {
    recordLog("FATAL ERROR: Could not create a GLFW window!");
    glfwTerminate();
    return false;
  }

  // Set GLFW variables
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);
  glfwSetKeyCallback(window, key_callback);
  glfwSetCursorPosCallback(window, cursor_callback);
  glfwSetWindowSizeCallback(window, window_resize_callback);
  if (hideCursor)
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  return true;
}

bool engine::checkVulkanValidationLayerSupport() {
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

bool engine::initVulkanInstance() {
  // Enable validation layers, if needed
  if (enableValidationLayers && !checkVulkanValidationLayerSupport()) {
    recordLog("FATAL ERROR: Validation layers were requested but could not be generated");
    return false;
  }

  // Optional struct, but may help Vulkan optimize
  VkApplicationInfo appInfo = {};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = windowTitle.c_str();
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

bool engine::initVulkanDebug() {
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

bool engine::initVulkanSurface() {
  VkResult result = glfwCreateWindowSurface(vulkanInstance, window, nullptr, &surface);
  if (result != VK_SUCCESS) {
    recordLog("FATAL ERROR: Could not create window surface!");
    return false;
  }

  return true;
}

engine::QueueFamilyIndices engine::findQueueFamilies(const VkPhysicalDevice& pDevice) {
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

engine::SwapChainSupportDetails engine::querySwapChainSupport(const VkPhysicalDevice& pDevice) {
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

bool engine::isDeviceSuitable(const VkPhysicalDevice& pDevice) {
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

bool engine::initVulkanDevice() {
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

bool engine::initVulkanQueues() {
  vkGetDeviceQueue(device, queueIndices.graphicsFamily, 0, &graphicsQueue);
  vkGetDeviceQueue(device, queueIndices.presentFamily, 0, &presentQueue);

  return true;
}

VkSurfaceFormatKHR engine::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats) {
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

VkPresentModeKHR engine::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& presentModes) {
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

VkExtent2D engine::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
  if (capabilities.currentExtent.width != numeric_limits<uint32_t>::max()) {
    return capabilities.currentExtent;
  }

  int width, height;
  glfwGetWindowSize(window, &width, &height);

  VkExtent2D actualExtent = {width, height};
  return actualExtent;
}

bool engine::initVulkanSwapChain() {
  VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(details.formats);
  VkPresentModeKHR presentMode = chooseSwapPresentMode(details.presentModes);
  VkExtent2D extent = chooseSwapExtent(details.capabilities);

  // Determine number of images in swap chain
  uint32_t imageCount = details.capabilities.minImageCount + 1;
  if (details.capabilities.maxImageCount > 0 && imageCount > details.capabilities.maxImageCount) {
    imageCount = details.capabilities.maxImageCount;
  }

  // Struct to create swap chain
  uint32_t indices[] = {queueIndices.graphicsFamily, queueIndices.presentFamily};
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

bool engine::initVulkanImageViews() {
  swapChainImageViews.resize(swapChainImages.size());

  for (int i = 0; i < swapChainImages.size(); i++) {
    VkImageView imageView;
    createImageView(swapChainImages[i], VK_IMAGE_VIEW_TYPE_2D, swapChainImageFormat, imageView);
    swapChainImageViews[i] = imageView;
  }

  return true;
}

bool engine::initVulkanRenderPass() {
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

  VkSubpassDescription subpass = {};
  subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
  subpass.colorAttachmentCount = 1;
  subpass.pColorAttachments = &colorAttachmentRef;

  VkSubpassDependency dependency = {};
  dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
  dependency.dstSubpass = 0;
  dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  dependency.srcAccessMask = 0;
  dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

  // Render pass creation info
  VkRenderPassCreateInfo renderPassInfo = {};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  renderPassInfo.attachmentCount = 1;
  renderPassInfo.pAttachments = &colorAttachment;
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

bool engine::initVulkanFramebuffers() {
  swapChainFramebuffers.resize(swapChainImageViews.size());

  for (int i = 0; i < swapChainImageViews.size(); i++) {
    VkImageView view = swapChainImageViews[i];
    VkImageView attachments[] = {
      view
    };

    VkFramebufferCreateInfo framebufferInfo = {};
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass = renderPass;
    framebufferInfo.attachmentCount = 1;
    framebufferInfo.pAttachments = attachments;
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

bool engine::initVulkanCommandPool() {
  VkCommandPoolCreateInfo poolInfo = {};
  poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  poolInfo.queueFamilyIndex = queueIndices.graphicsFamily;
  poolInfo.flags = 0;

  VkResult result = vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool);
  if (result != VK_SUCCESS) {
    recordLog("FATAL ERROR: Could not create Vulkan command pool");
    return false;
  }

  return true;
}

bool engine::initVulkanCommandBuffers() {
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

bool engine::initVulkanSemaphores() {
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

bool engine::initVulkan() {
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

  if (!initVulkanFramebuffers())
    return false;

  if (!initVulkanCommandPool())
    return false;

  if (!initVulkanCommandBuffers()) 
    return false;

  if (!initVulkanSemaphores())
    return false;

  return true;
}

bool engine::createVulkanBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) {
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
    recordLog("ERROR: Failed to allocate vertex buffer memory");
    return false;
  }
  vkBindBufferMemory(device, buffer, bufferMemory, 0);  

  return true;
}

bool engine::createVulkanImage(uint32_t width, uint32_t height, uint32_t layers, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory) {
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
}

bool engine::copyVulkanBuffer(VkBuffer src, VkBuffer dst, VkDeviceSize size) {
  VkCommandBuffer commandBuffer = beginSingleTimeCommands();

  VkBufferCopy copyRegion = {};
  copyRegion.srcOffset = 0;
  copyRegion.dstOffset = 0;
  copyRegion.size = size;
  vkCmdCopyBuffer(commandBuffer, src, dst, 1, &copyRegion);

  endSingleTimeCommands(commandBuffer);

  return true;
}

void engine::cleanupSwapChain() {
  for (auto buffer : swapChainFramebuffers) {
    vkDestroyFramebuffer(device, buffer, nullptr);
  }

  vkFreeCommandBuffers(device, commandPool, commandBuffers.size(), commandBuffers.data());

  resourceHandler::getInstance()->unloadShaders();
  vkDestroyRenderPass(device, renderPass, nullptr);

  for (auto view : swapChainImageViews) {
    vkDestroyImageView(device, view, nullptr);
  }

  vkDestroySwapchainKHR(device, swapChain, nullptr);
}

// Recreate swap chain if needed
bool engine::recreateSwapChain() {
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
  
  if (!initVulkanRenderPass())
    return false;

  resourceHandler::getInstance()->reloadShaders();

  if (!initVulkanFramebuffers())
    return false;

  if (!initVulkanCommandBuffers())
    return false;

  return true;
}

// Start the graphics system
bool engine::initGraphics() {
  if (!initGLFW())
    return false;

  if (!initVulkan())
    return false;

  setDefaultAspectRatio((double) windowWidth / (double) windowHeight);
  return true;
}

// Prepares the game to run
bool engine::initGame() {
  srand(time(nullptr));
  if (!initGraphics())
    return false;

  resourceHandler::getInstance()->setDefaultShaderProg(vertexShader, fragmentShader);

  scene::setActiveScene(new main_scene());
  scene::getActiveScene()->load();

  return true;
}

// Main drawing function
void engine::draw() {
  double currentTime = glfwGetTime();
  
  for (int i = 0; i < commandBuffers.size(); i++) {
    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
    beginInfo.pInheritanceInfo = nullptr;

    vkBeginCommandBuffer(commandBuffers[i], &beginInfo);

    VkClearValue clearColor = {0.0f, 0.0f, 0.0f, 1.0f};
    VkRenderPassBeginInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = renderPass;
    renderPassInfo.framebuffer = swapChainFramebuffers[i];
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = swapChainExtent;
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
  }

  scene::getActiveScene()->draw();

  for (auto buffer : commandBuffers) {
    vkCmdEndRenderPass(buffer);
    VkResult result = vkEndCommandBuffer(buffer);
    if (result != VK_SUCCESS) {
      recordLog("FATAL ERROR: Failed to record command buffer!");
      return;
    }
  }

  uint32_t imageIndex;
  VkResult result = vkAcquireNextImageKHR(device, swapChain, numeric_limits<uint64_t>::max(), imageAvailable, VK_NULL_HANDLE, &imageIndex);
  if (result == VK_ERROR_OUT_OF_DATE_KHR) {
    recreateSwapChain();
    return;
  } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
    recordLog("FATAL ERROR: Failed to acquire swap chain image");
    return;
  }

  VkSemaphore waitSemaphores[] = {imageAvailable};
  VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
  VkSemaphore signalSemaphores[] = {renderFinished};
  VkSubmitInfo submitInfo = {};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submitInfo.waitSemaphoreCount = 1;
  submitInfo.pWaitSemaphores = waitSemaphores;
  submitInfo.pWaitDstStageMask = waitStages;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &commandBuffers[imageIndex];
  submitInfo.signalSemaphoreCount = 1;
  submitInfo.pSignalSemaphores = signalSemaphores;

  result = vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
  if (result != VK_SUCCESS) {
    recordLog("FATAL ERROR: Could not draw command buffer");
    return;
  }

  VkSwapchainKHR swapChains[] = {swapChain};
  VkPresentInfoKHR presentInfo = {};
  presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  presentInfo.waitSemaphoreCount = 1;
  presentInfo.pWaitSemaphores = signalSemaphores;
  presentInfo.swapchainCount = 1;
  presentInfo.pSwapchains = swapChains;
  presentInfo.pImageIndices = &imageIndex;
  presentInfo.pResults = nullptr;

  result = vkQueuePresentKHR(presentQueue, &presentInfo);
  if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
    recreateSwapChain();
  } else if (result != VK_SUCCESS) {
    recordLog("FATAL ERROR: Failed to present swap chain image!");
    return;
  }
}

bool engine::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, uint32_t* output) {
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

VkCommandBuffer engine::beginSingleTimeCommands() {
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

void engine::endSingleTimeCommands(VkCommandBuffer commandBuffer) {
  vkEndCommandBuffer(commandBuffer);

  VkSubmitInfo submitInfo = {};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &commandBuffer;

  vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
  vkQueueWaitIdle(graphicsQueue);

  vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
}

bool engine::createImageView(VkImage image, VkImageViewType viewType, VkFormat format, VkImageView& imageView) {
  VkImageViewCreateInfo createInfo = {};
  createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
  createInfo.image = image;
  createInfo.viewType = viewType;
  createInfo.format = format;
  createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  createInfo.subresourceRange.baseMipLevel = 0;
  createInfo.subresourceRange.levelCount = 1;
  createInfo.subresourceRange.baseArrayLayer = 0;
  createInfo.subresourceRange.layerCount = 1;

  VkResult result = vkCreateImageView(device, &createInfo, nullptr, &imageView);
  if (result != VK_SUCCESS) {
    recordLog("FATAL ERROR: Could not create image views");
    return false;
  }

  return true;
}

void engine::copyBufferToImage(VkBuffer buffer, VkImage image, vector<VkBufferImageCopy> bufferCopies) {
  VkCommandBuffer commandBuffer = beginSingleTimeCommands();
  vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, bufferCopies.data());
  endSingleTimeCommands(commandBuffer);
}

void engine::transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout) {
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

  else {
    recordLog("ERROR: Unsupported layout transition");
    return;
  }

  vkCmdPipelineBarrier(commandBuffer, sourceStage, destinationStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);

  endSingleTimeCommands(commandBuffer);
}

// Run the game
void engine::runGame() {
  // Make sure the game is initialized
  if (!initGame()) {
    cerr << "A fatal error occurred. Check the log for more details." << endl;
    return;
  }

  // Main game loop
  while (!glfwWindowShouldClose(window)) {
    if(scene::getActiveScene()->update() == CHANGE_SCENE) {
	    scene* nextScene = scene::getActiveScene()->nextScene();
      nextScene->load();

	    scene::endActiveScene();
      scene::setActiveScene(nextScene);
      nextScene->update();
    }
      
    vkDeviceWaitIdle(device);
    draw();
    glfwSwapBuffers(window); // TODO: remove?
    glfwPollEvents();
  }

  vkDeviceWaitIdle(device);
}
