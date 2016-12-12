#pragma once

#include "log.hpp"
#include <string>

template <class T>
resource<T>::resource(T* res, resourceHandler* rHandler) {
  this->res = res;
  res->load();
  this->rHandler = rHandler;
}

template <class T>
resource<T>::~resource() {
  int numRefs = res->unload();
  if (numRefs == 0) {
    rHandler->unload(res->getName());
  }
}

template <class T>
void resource<T>::operator = (const resource<T>& rhs) {
  res = rhs.res;
  res->load();
  rHandler = rhs.rHandler;
}
