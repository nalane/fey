#pragma once

/*
 * Resource base class; primarily manages ref count
 */

#include <string>

class resource {
protected:
  int refCount;

public:
  resource() : refCount(0) { }

  void incRef();
  void decRef();
};
