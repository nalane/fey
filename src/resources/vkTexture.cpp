#include "vkTexture.hpp"
#include "log.hpp"
#include "graphics.hpp"

using namespace std;

vkTexture::vkTexture(const string& name, const set<string>& files) : texture(name, files) {
    // Set graphics engine for easy access
    graphicsEngine = (vulkan*)graphics::getInstance();
}

vkTexture::~vkTexture() {
    VkDevice device = graphicsEngine->getDevice();

    vkDestroySampler(device, textureSampler, nullptr);
    vkDestroyImageView(device, textureImageView, nullptr);

    vkFreeMemory(device, textureImageMemory, nullptr);
    vkDestroyImage(device, textureImage, nullptr);
}

bool vkTexture::loadTexture() {
    VkDevice device = graphicsEngine->getDevice();

    int layer = 0;
    VkDeviceSize offset = 0;
    vector<stbi_uc> buffer;
    vector<int> widths, heights, channels;
    vector<VkBufferImageCopy> copyRegions;
    for (string file : files) {
        // Load image
        int width, height, numChannels;
        stbi_uc* pixels = stbi_load(file.c_str(), &width, &height, &numChannels, STBI_rgb_alpha);
        VkDeviceSize imageSize = width * height * STBI_rgb_alpha;

        // Make sure image is loaded
        if (!pixels) {
            recordLog("ERROR: Failed to load texture image " + file);
            return false;
        }

        widths.push_back(width);
        heights.push_back(height);
        channels.push_back(numChannels);
        buffer.insert(buffer.end(), pixels, pixels + imageSize);

        VkBufferImageCopy region = {};
        region.bufferOffset = offset;
        region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        region.imageSubresource.mipLevel = 0;
        region.imageSubresource.baseArrayLayer = layer;
        region.imageSubresource.layerCount = 1;
        region.imageExtent = {static_cast<uint32_t>(width), static_cast<uint32_t>(height), 1};

        copyRegions.push_back(region);

        stbi_image_free(pixels);
        layer++;
        offset += imageSize;
    }

    // Load into staging buffer
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    graphicsEngine->createVulkanBuffer(buffer.size(), VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    // Move data to buffer
    void* data;
    vkMapMemory(device, stagingBufferMemory, 0, buffer.size(), 0, &data);
    memcpy(data, buffer.data(), buffer.size());
    vkUnmapMemory(device, stagingBufferMemory);

    // Move texture to GPU memory
    VkImageCreateFlags flags = files.size() == 6 ? VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT : 0;
    graphicsEngine->createVulkanImage(widths[0], heights[0], files.size(), VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, flags, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, textureImage, textureImageMemory);
    graphicsEngine->transitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    graphicsEngine->copyBufferToImage(stagingBuffer, textureImage, copyRegions);
    graphicsEngine->transitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    // Clear staging buffers
    vkFreeMemory(device, stagingBufferMemory, nullptr);
    vkDestroyBuffer(device, stagingBuffer, nullptr);

    // Create image view
    VkImageViewType viewType = files.size() == 6 ? VK_IMAGE_VIEW_TYPE_CUBE : VK_IMAGE_VIEW_TYPE_2D;
    graphicsEngine->createImageView(textureImage, viewType, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_ASPECT_COLOR_BIT, files.size(), textureImageView);

    // Create sampler
    VkSamplerCreateInfo samplerInfo = {};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = VK_FILTER_LINEAR;
    samplerInfo.minFilter = VK_FILTER_LINEAR;
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerInfo.anisotropyEnable = VK_TRUE;
    samplerInfo.maxAnisotropy = 16;
    samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerInfo.mipLodBias = 0.0f;
    samplerInfo.minLod = 0.0f;
    samplerInfo.maxLod = 0.0f;

    VkResult result = vkCreateSampler(device, &samplerInfo, nullptr, &textureSampler);
    if (result != VK_SUCCESS) {
        recordLog("ERROR: Could not create texture sampler");
        return false;
    }

    return true;
}