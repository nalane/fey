#pragma once

#include <string>
#include <map>

class model;
class resourceHandler;
#include "model.hpp"
#include "glHeaders.hpp"
#include "shader.hpp"
#include "shaderProgram.hpp"
#include "camera.hpp"

class resourceHandler {
private:
	std::map<std::string, model*> models;
	std::map<std::string, shader*> vertexShaders;
	std::map<std::string, shader*> fragmentShaders;
	std::map<std::string, shaderProgram*> shaderPrograms;
	std::map<std::string, camera*> cameras;

	model* loadFeyModel(std::string filename);
	shader* loadVertexShader(std::string vertexShaderPath);
	shader* loadFragmentShader(std::string fragmentShaderPath);

public:
	resourceHandler() {};
	~resourceHandler();

	model* loadModel(std::string filepath);
	shaderProgram* loadShaderProg(std::string vertexShader, std::string fragmentShader);

	void unloadAll();

	void setCamera(std::string id, camera* cam);
	camera* getCamera(std::string id);
};