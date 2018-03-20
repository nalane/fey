#pragma once

/*
 * Base abstract class for texture resource
 */

#include "raw_resource.hpp"

#include <set>
#include <string>

class texture : public raw_resource {
protected:
    std::set<std::string> files;

public:
    texture(const std::string& name, const std::set<std::string>& files) : raw_resource(name), files(files) { }
    virtual ~texture() { }

    static texture* createTexture(const std::string& name, const std::set<std::string>& files);

    virtual bool loadTexture() = 0;
};