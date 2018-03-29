#include "raw_resource.hpp"
#include "log.hpp"

using namespace std;

GLuint raw_resource::uniformBindingPoint = 0;

raw_resource::~raw_resource() {

}

void raw_resource::load() {
  locker.lock();
  refCount++;
  locker.unlock();

  for (auto bins : child_resources)
    for (auto p : bins.second)
      p.second->load();
}

vector<string> raw_resource::unload() {
  vector<string> unloadResources;
  
  locker.lock();
  refCount--;
  locker.unlock();

  if (refCount <= 0)
    unloadResources.push_back(name);

  for (auto bins : child_resources) {
    for (auto p : bins.second) {
      vector<string> childUnloads = p.second->unload();

      // Children must be unloaded first
      for (string r : unloadResources)
        childUnloads.push_back(r);
        
      unloadResources = childUnloads;
    }
  }
  
  return unloadResources;
}
