#pragma once

/*
 * Resource base class; primarily used as base class
 */

#include <string>
#include <mutex>
#include <map>
#include <set>

class raw_resource {
protected:
  int refCount;
  std::string name;
  std::mutex locker;

  std::map<std::string, raw_resource*> child_resources;
public:
  raw_resource(std::string name) : refCount(0), name(name) { }
  virtual ~raw_resource();
  
  void load();
  std::set<std::string> unload();

  std::string getName() { return name; }
};
