#pragma once

/*
 * Resource base class; essentially a reference in other langs
 */

#include <string>
#include <mutex>
#include <map>
#include <vector>

class raw_resource {
protected:
  int refCount; // Number of times this resource is referenced
  std::string name; // Name in rHandler
  std::mutex locker;

  std::map<std::string, raw_resource*> child_resources;

public:
  raw_resource(const std::string& name) : refCount(0), name(name) { }
  virtual ~raw_resource();
  
  void load();
  std::vector<std::string> unload();

  std::string getName() const { return name; }
};
