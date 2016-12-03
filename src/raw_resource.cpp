#include "raw_resource.hpp"

raw_resource::~raw_resource() {

}

void raw_resource::load() {
  refCount++;
}

int raw_resource::unload() {
  return --refCount;
}
