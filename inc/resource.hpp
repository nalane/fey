#pragma once

/*
 * Resource wrapper.
 */

class resourceHandler;

template <class T>
class resource {
private:
  resourceHandler* rHandler;
  
public:
  T* res;

  resource() : rHandler(nullptr), res(nullptr) {}
  resource(T* res, resourceHandler* rHandler);

  void operator = (const resource<T>& rhs);
  
  ~resource();
};

#include "resourceHandler.hpp"
