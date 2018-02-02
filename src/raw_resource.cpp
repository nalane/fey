#include "raw_resource.hpp"
#include "log.hpp"

using namespace std;

raw_resource::~raw_resource() {

}

void raw_resource::load() {
  locker.lock();
  refCount++;
  locker.unlock();

  for (auto p : child_resources)
    p.second->load();
}

vector<string> raw_resource::unload() {
  vector<string> unloadResources;
  
  locker.lock();
  refCount--;
  locker.unlock();

  if (refCount <= 0)
    unloadResources.push_back(name);

  for (auto p : child_resources) {
    vector<string> childUnloads = p.second->unload();
    for (string r : unloadResources)
      childUnloads.push_back(r);
    unloadResources = childUnloads;
  }
  
  return unloadResources;
}
