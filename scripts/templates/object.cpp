#include "@name@.hpp"
#include "paths.hpp"

using namespace std;

@name@::@name@(resourceHandler* rHandler) : object(rHandler) {
}

@name@::~@name@() {

}

void @name@::load() {
  object::load();
}

void @name@::init() {
  object::init();
}

void @name@::update() {
  object::update();
}

void @name@::draw() {
  object::draw();
  m.res->bindTextureToUniform(texHandle);
  m.res->draw();
}
