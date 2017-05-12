#pragma once

/*
 * Handles all data read in from a file and stores it in a resource
 */

#include <vector>
#include <string>
#include <map>

template <typename T> class resource;
class model;
class resourceHandler;
class material;
class skybox;

#include "resource.hpp"
#include "glHeaders.hpp"
#include "shader.hpp"
#include "shaderProgram.hpp"
#include "camera.hpp"
#include "model.hpp"
#include "skybox.hpp"
#include "raw_resource.hpp"
#include "light.hpp"
#include "firstPersonCamera.hpp"
#include "trackBallCamera.hpp"

class resourceHandler {
private:
  std::map<std::string, raw_resource*> resources;
  std::map<std::string, camera*> cameras;
  std::map<std::string, light*> lights;

  std::string activeCameraID;

  std::string getShaderKey(const std::string& vert, const std::string& frag) const
	{ return "v" + vert + "f" + frag; }
  model* loadFeyModel(const std::string& filename);
  shader* loadVertexShader(const std::string& vertexShaderPath);
  shader* loadFragmentShader(const std::string& fragmentShaderPath);
  shaderProgram* newShader(const std::string& vertexShader, const std::string& fragmentShader, const std::string& key);

public:
  resourceHandler() {};
  ~resourceHandler();

  resource<model> loadModel(const std::string& filepath);
  resource<light> loadLight(const std::string& lightName);
  resource<camera> loadCamera(const std::string& name);
  resource<firstPersonCamera> loadFirstPersonCamera(const std::string& name);
  resource<trackBallCamera> loadTrackBallCamera(const std::string& name);
  resource<shaderProgram> loadShaderProg(const std::string& vertexShader, const std::string& fragmentShader,
					 bool defaultShader = false);
  resource<shaderProgram> loadShaderProg();
  resource<skybox> loadSkybox(const std::string& path, const std::string& extension);

  void unload(const std::string& name);
  void unloadAll();
  
  std::vector<light*> getAllLights() const;

  void setActiveCamera(const std::string& id);
  camera* getActiveCamera() const;
};

#include "resource.tpp"
