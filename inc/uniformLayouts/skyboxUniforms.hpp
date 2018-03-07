#pragma once

#include "glHeaders.hpp"

#define ALIGNMENT 16

struct alignas(ALIGNMENT) skyboxUniforms {
    glm::mat4 vpMatrix;
};