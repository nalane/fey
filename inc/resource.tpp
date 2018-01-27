#pragma once

#include "log.hpp"
#include "resourceHandler.hpp"
#include <string>
#include <vector>

// Constructor with input resource data
template <class T>
resource<T>::resource(T* res) {
  this->res = res;
  if (res != nullptr) {
    res->load();
  }
}

// Copy constructor
template <class T>
resource<T>::resource(const resource& rhs) {
  res = rhs.res;
  if (res != nullptr) {
    res->load();
  }
}

// Equals operator
template <class T>
void resource<T>::operator = (const resource<T>& rhs) {
  unload();
  res = rhs.res;
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
      resourceHandler::getInstance()->unload(s);
    }
  }
}
