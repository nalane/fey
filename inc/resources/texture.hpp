#pragma once

/*
 * Contains code for storing a texture
 */

#include <string>
#include <set>

#include "raw_resource.hpp"
#include "glHeaders.hpp"

class texture : public raw_resource {
private:
    VkImage textureImage;
    VkDeviceMemory textureImageMemory;

    VkImageView textureImageView;
    VkSampler textureSampler;

    std::set<std::string> files;

public:
    texture(const std::string& name, std::set<std::string> files) : raw_resource(name), files(files) { }
    ~texture();

    bool loadTexture();

    VkSampler getSampler() { return textureSampler; }
    VkImageView getImageView() { return textureImageView; }

    //void draw(GLuint index);
};