#pragma once

/*
 * Resource base class; primarily used as base class
 */

#include <string>
#include <mutex>
#include <map>
#include <vector>

class raw_resource {
protected:
  int refCount;
  std::string name;
  std::mutex locker;

  std::map<std::string, raw_resource*> child_resources;
public:
  raw_resource(const std::string& name) : refCount(0), name(name) { }
  virtual ~raw_resource();
  
  void load();
  std::vector<std::string> unload();

  std::string getName() const { return name; }
};
