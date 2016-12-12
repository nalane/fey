#pragma once

/*
 * Resource wrapper.
 */

template <class T>
class resource;

#include "resourceHandler.hpp"

template <class T>
class resource {
private:
  resourceHandler* rHandler;
  
public:
  T* res;

  resource() : rHandler(NULL), res(NULL) {}
  resource(T* res, resourceHandler* rHandler);

  void operator = (const resource<T>& rhs);
  
  ~resource();
};
