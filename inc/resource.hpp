#pragma once

/*
 * Resource wrapper.
 */

template <class T>
class resource {
private:
  void unload();
  
public:
  // Pointer to actual resource data
  T* res;

  resource() : res(nullptr) { }
  resource(T* res);
  resource(const resource<T>& rhs);

  void operator = (const resource<T>& rhs);

  ~resource();
};

#ifndef NO_RESOURCE_TEMPLATE
#include "resource.tpp"
#endif
