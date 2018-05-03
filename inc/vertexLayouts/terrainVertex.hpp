#pragma once

#include "glHeaders.hpp"
#include <vector>

#define ALIGNMENT 16

struct alignas(ALIGNMENT) terrainVertex {
    glm::vec3 position;

    static void bindLayout() {
        // Position 
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE,
            sizeof(terrainVertex), (GLvoid*)(offsetof(terrainVertex, position)));
    }

    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription = {};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(terrainVertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions() {
        std::vector<VkVertexInputAttributeDescription> attributeDescriptions(1);

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32A32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(terrainVertex, position);

        return attributeDescriptions;
    }
};