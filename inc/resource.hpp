#pragma once

/*
 * Resource wrapper.
 */

template <class T>
class resource {
private:
  void unload();
  
public:
  T* res;

  resource() : res(nullptr) {}
  resource(T* res);
  resource(const resource<T>& rhs);

  void operator = (const resource<T>& rhs);

  ~resource();
};

#include "resource.tpp"
