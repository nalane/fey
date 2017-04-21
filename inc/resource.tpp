#pragma once

#include "log.hpp"
#include <string>
#include <vector>

template <class T>
resource<T>::resource(T* res, resourceHandler* rHandler) {
  this->res = res;
  this->rHandler = rHandler;
  if (res != nullptr) {
    res->load();
  }
}

template <class T>
resource<T>::resource(const resource& rhs) {
  res = rhs.res;
  rHandler = rhs.rHandler;
  if (res != nullptr) {
    res->load();
  }
}

template <class T>
void resource<T>::operator = (const resource<T>& rhs) {
  unload();
  res = rhs.res;
  rHandler = rhs.rHandler;
  if (res != nullptr) {
    res->load();
  }
}

template <class T>
resource<T>::~resource() {
  unload();
}

template <class T>
void resource<T>::unload() {
  if (res != nullptr) {
    // Get the list of resources to unload, and unload them
    std::vector<std::string> unloadResources = res->unload();
    for (std::string s : unloadResources) {
      rHandler->unload(s);
    }
  }
}
