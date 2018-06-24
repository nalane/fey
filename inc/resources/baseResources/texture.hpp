#pragma once

/*
 * Base abstract class for texture resource
 */

#include "raw_resource.hpp"

#include <set>
#include <string>

class texture : public raw_resource {
protected:  

public:
    texture(const std::string& name) : raw_resource(name) { }
    virtual ~texture() { }

    static texture* createTexture(const std::string& name, const std::set<std::string>& files);

    virtual void loadTexture() = 0;
};