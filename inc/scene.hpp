#pragma once

/*
 * Contains base class for a generic scene
 */

#include "resourceHandler.hpp"
#include "object.hpp"

#include <unordered_map>
#include <string>

class scene {
protected:
  resourceHandler* rHandler;
  std::unordered_map<std::string, object*> objects;
  
public:
  scene(resourceHandler* rHandler) : rHandler(rHandler) {};
  virtual ~scene() {};

  virtual void load() = 0;
  virtual void update() = 0;
  virtual void draw() = 0;
  virtual void unload() = 0;
};
