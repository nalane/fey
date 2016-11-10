#include "resource.hpp"

// Resource is used in another object
void resource::incRef() {
  refCount++;
}

// Resource is no longer needed in an object
void resource::decRef() {
  refCount--;
}
