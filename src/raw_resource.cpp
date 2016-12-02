#include "raw_resource.hpp"

void raw_resource::load() {
  refCount++;
}

int raw_resource::unload() {
  return --refCount;
}
