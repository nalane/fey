#pragma once

#include "log.hpp"
#include <string>
#include <set>

template <class T>
resource<T>::resource(T* res, resourceHandler* rHandler) {
  this->res = res;
  this->rHandler = rHandler;
  if (res != nullptr) {
    res->load();
  }
}

template <class T>
resource<T>::~resource() {
  if (res != nullptr) {
    std::set<std::string> unloadResources = res->unload();
    for (std::string s : unloadResources) {
      rHandler->unload(s);
    }
  }
}

template <class T>
void resource<T>::operator = (const resource<T>& rhs) {
  res = rhs.res;
  rHandler = rhs.rHandler;
  if (res != nullptr) {
    res->load();
  }
}
