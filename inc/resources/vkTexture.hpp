#pragma once

/*
 * Contains code for storing a texture
 */

#include <string>
#include <set>

#include "texture.hpp"
#include "glHeaders.hpp"
#include "vulkan.hpp"

class vkTexture : public texture {
private:
    vulkan* graphicsEngine;

    VkImage textureImage;
    VkDeviceMemory textureImageMemory;

    VkImageView textureImageView;
    VkSampler textureSampler;

public:
    vkTexture(const std::string& name, const std::set<std::string>& files);
    ~vkTexture();

    virtual bool loadTexture();

    VkSampler getSampler() { return textureSampler; }
    VkImageView getImageView() { return textureImageView; }
};