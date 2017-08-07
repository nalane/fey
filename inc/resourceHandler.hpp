#pragma once

/*
 * Handles all data read in from a file and stores it in a resource
 */

#include <vector>
#include <string>

void endResourceHandler();

void rhUnload(const std::string& name);
void rhUnloadAll();

#include "resource.hpp"
#include "shader.hpp"
#include "shaderProgram.hpp"
#include "camera.hpp"
#include "model.hpp"
#include "skybox.hpp"
#include "light.hpp"
#include "firstPersonCamera.hpp"
#include "trackBallCamera.hpp"

void setDefaultShaderProg(const std::string& vertexShader, const std::string& fragmentShader);
resource<shaderProgram> loadShaderProg(const std::string& vertexShader, const std::string& fragmentShader);
resource<shaderProgram> loadShaderProg();
  
resource<model> loadModel(const std::string& filepath);
resource<light> loadLight(const std::string& lightName);
resource<skybox> loadSkybox(const std::string& path, const std::string& extension);

resource<camera> loadCamera(const std::string& name);
resource<firstPersonCamera> loadFirstPersonCamera(const std::string& name);
resource<trackBallCamera> loadTrackBallCamera(const std::string& name);
  
std::vector<light*> getAllLights();

void setActiveCamera(const std::string& id);
camera* getActiveCamera();
