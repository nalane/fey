#pragma once

/*
 * Resource base class; primarily used as base class
 */

#include <string>

class raw_resource {
protected:
  int refCount;
  std::string name;
  
public:
  raw_resource(std::string name) : refCount(0), name(name) { }
  virtual ~raw_resource();
  
  void load();
  int unload();

  std::string getName() { return name; }
};
