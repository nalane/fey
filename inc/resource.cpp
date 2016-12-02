#pragma once

#include <iostream>

template <class T>
resource<T>::resource(T* res, resourceHandler* rHandler) {
  this->res = res;
  res->load();
  this->rHandler = rHandler;
}

template <class T>
resource<T>::~resource() {
  if (res->unload() == 0) {
    rHandler->unload(res->getName());
  }
}
