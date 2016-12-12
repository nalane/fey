#include "raw_resource.hpp"
#include "log.hpp"

using namespace std;

raw_resource::~raw_resource() {

}

void raw_resource::load() {
  locker.lock();
  refCount++;
  locker.unlock();
}

int raw_resource::unload() {
  locker.lock();
  refCount--;
  locker.unlock();
  return refCount;
}
