#pragma once

#include "log.hpp"
#include <string>

template <class T>
resource<T>::resource(T* res, resourceHandler* rHandler) {
  this->res = res;
  this->rHandler = rHandler;
  if (res != NULL) {
    res->load();
  }
}

template <class T>
resource<T>::~resource() {
  if (res != NULL) {
    int numRefs = res->unload();
    if (numRefs == 0) {
      rHandler->unload(res->getName());
    }
  }
}

template <class T>
void resource<T>::operator = (const resource<T>& rhs) {
  res = rhs.res;
  rHandler = rhs.rHandler;
  if (res != NULL) {
    res->load();
  }
}
