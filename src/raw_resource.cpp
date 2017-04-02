#include "raw_resource.hpp"
#include "log.hpp"

using namespace std;

raw_resource::~raw_resource() {

}

void raw_resource::load() {
  locker.lock();
  refCount++;
  locker.unlock();

  for (auto p : child_resources) {
    p.second->load();
  }
}

set<string> raw_resource::unload() {
  set<string> unloadResources;
  
  locker.lock();
  refCount--;
  locker.unlock();

  if (refCount <= 0)
    unloadResources.insert(name);

  for (auto p : child_resources) {
    for (string r : p.second->unload())
      unloadResources.insert(name);
  }
  
  return unloadResources;
}
