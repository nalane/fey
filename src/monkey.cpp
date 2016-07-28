#include "monkey.hpp"
#include "paths.hpp"

using namespace std;

monkey::monkey(resourceHandler* rHandler, string vertexShader, string fragmentShader) 
	: object(rHandler, vertexShader, fragmentShader) {
	
}

void monkey::load() {
	object::load();
	m = rHandler->loadModel(getLibraryFolderPath("monkey.fey.model"));
  	modelMatrix = glm::rotate(glm::radians(-90.0f),
			   				  glm::vec3(1.0, 0.0, 0.0)) * modelMatrix;
}

void monkey::init() {
	object::update();
}

void monkey::update() {
	object::update();
	modelMatrix = glm::rotate(glm::radians(0.2f),
			    			  glm::vec3(0.0, 1.0, 0.0)) * modelMatrix;
}

void monkey::draw() {
	object::draw();
	m->bindTextureToUniform(texHandle);
  	m->draw();
}