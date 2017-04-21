#pragma once

/*
 * Resource wrapper.
 */

class resourceHandler;

template <class T>
class resource {
private:
  resourceHandler* rHandler;
  void unload();
  
public:
  T* res;

  resource() : rHandler(nullptr), res(nullptr) {}
  resource(T* res, resourceHandler* rHandler);
  resource(const resource<T>& rhs);

  void operator = (const resource<T>& rhs);

  ~resource();
};

#include "resourceHandler.hpp"
