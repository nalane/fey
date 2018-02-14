#pragma once

/*
 * Contains code for storing a texture
 */

#include <string>

#include "raw_resource.hpp"
#include "glHeaders.hpp"

class texture : public raw_resource {
private:
    GLuint texID;

public:
    texture(const std::string& name);
    ~texture();

    void draw(GLuint index);
};