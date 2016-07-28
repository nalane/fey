#pragma once

#include "object.hpp"
#include "model.hpp"
#include "resourceHandler.hpp"
#include "shaderProgram.hpp"
#include "glHeaders.hpp"

#include <string>

class monkey : public object {
private:
	model* m;

public:
	monkey(resourceHandler* rHandler, std::string vertexShader, std::string fragmentShader);

	void load();
	void init();
  	void update();
  	void draw();
};