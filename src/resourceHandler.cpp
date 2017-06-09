#include "resourceHandler.hpp"

#include <iostream>
#include <fstream>
#include <map>
#include <cstdlib>
#include <string>
#include <set>

#include "paths.hpp"
#include "log.hpp"
#include "glHeaders.hpp"

using namespace std;

const string SHADER_KEY = "___MAIN_SHADER_KEY___";

resourceHandler::~resourceHandler() {
  unloadAll();
}

// Loads a fey model into memory and returns the data
model* resourceHandler::loadFeyModel(const string& filename) {
  string fullPath = getLibraryFolderPath(filename);
  recordLog("Reading fey model " + fullPath + "...");
  model* m = nullptr;
  ifstream fin(fullPath.c_str());

  if (fin.is_open()) {
    m = new model(filename);

    // Get version number and number of materials
    string version = "";
    int numMaterials = 0;
    fin >> version;
    if (version.substr(0, 1) == "v") {
      fin >> numMaterials;
    }
    else {
      numMaterials = stoi(version);
    }
    

    // Get materials
    for (int i = 0; i < numMaterials; i++) {
      glm::vec4 amb(0.0);
      glm::vec4 diffuse(1.0);
      glm::vec4 specular(1.0);
      float specularIntensity;

      fin >> diffuse[0] >> diffuse[1] >> diffuse[2];
      fin >> specular[0] >> specular[1] >> specular[2];
      fin >> specularIntensity;

      m->addMaterial(material(amb, diffuse, specular, specularIntensity));
    }
    
    // Get number of vertices
    int numVertices = 0;
    fin >> numVertices;

    // Read all vertices
    vector<glm::vec3> vertexList;
    for (int i = 0; i < numVertices; i++) {
      float x, y, z;

      fin >> x;
      fin >> y;
      fin >> z;

      vertexList.push_back(glm::vec3(x, y, z));
    }
		
    // Get the number of textures
    int numTextures = 0;
    fin >> numTextures;

    // Get the texture data for each texture
    vector<glm::vec2> uvCoords;
    for (int i = 0; i < numTextures; i++) {
      string filename = "junk";
      int numUVCoords = -1;
			
      getline(fin, filename);
      getline(fin, filename);
      fin >> numUVCoords;
			
      for (int j = 0; j < numUVCoords; j++) {
	int uvIdx;
	float u, v;
				
	fin >> uvIdx;
	fin >> u;
	fin >> v;
			
	uvCoords.push_back(glm::vec2(u, v));
      }
		
      m->setTexture(getLibraryFolderPath(filename));
    }

    // Number of vertices in final model
    int numVerts = 0;
    fin >> numVerts;
    int factor = version.substr(0, 1) == "v" ? 5 : 2;
    numVerts /= factor;
		
    // Get the UV map
    vector<glm::vec2> uvMapping;
    vector<glm::vec3> finalVerts;
    vector<glm::vec3> normals;
    glm::vec3 face[3];
    for(int i = 0; i < numVerts; i++) {
      int index, uvIndex;
			
      fin >> index;
      finalVerts.push_back(vertexList[index]);

      fin >> uvIndex;
      if (uvCoords.size() > 0) {
	uvMapping.push_back(uvCoords[uvIndex]);
      }

      if (version.substr(0, 1) != "v") {
	face[i % 3] = vertexList[index];
	if (i % 3 == 2) {
	  glm::vec3 normal = glm::cross(face[1] - face[0], face[2] - face[0]);
	  normals.push_back(normal);
	  normals.push_back(normal);
	  normals.push_back(normal);
	}
      }

      else {
	glm::vec3 normal;
	fin >> normal.x >> normal.y >> normal.z;
	normals.push_back(normal);
      }
    }

    // Push data into the model
    m->setVertices(finalVerts);
    m->setNormals(normals);
    if (uvMapping.size() > 0)
      m->setUVMapping(uvMapping);
    
    recordLog("Successfully read in fey model file " + filename + "!");
  }

  else {
    recordLog("Could not read file: " + filename);
  }

  return m;
}

// Get the model associated with the given filename
resource<model> resourceHandler::loadModel(const string& filepath) {
  if (resources.find(filepath) == resources.end())
    resources[filepath] = loadFeyModel(filepath);

  return resource<model>((model*) resources[filepath], this);
}

// Get a vertex shader
shader* resourceHandler::loadVertexShader(const string& vertexShaderPath) {
  if (resources.find(vertexShaderPath) == resources.end())
    resources[vertexShaderPath] = new shader(vertexShaderPath, GL_VERTEX_SHADER);

  return (shader*) resources[vertexShaderPath];
}

// Get a fragment shader
shader* resourceHandler::loadFragmentShader(const string& fragmentShaderPath) {
  if (resources.find(fragmentShaderPath) == resources.end())
    resources[fragmentShaderPath] = new shader(fragmentShaderPath, GL_FRAGMENT_SHADER);

  return (shader*) resources[fragmentShaderPath];
}

// Create a new shader program
shaderProgram* resourceHandler::newShader(const string& vertexShader, const string& fragmentShader, const string& key) {
  shaderProgram* prog = new shaderProgram(key);
  prog->addShader(loadVertexShader(vertexShader));
  prog->addShader(loadFragmentShader(fragmentShader));
  prog->loadShaders();
  prog->compileShaders();
  prog->linkShaders();
  return prog;
}

// Get a shader program
resource<shaderProgram> resourceHandler::loadShaderProg(const string& vertexShader, const string& fragmentShader, bool defaultShader) {
  string key = defaultShader ? SHADER_KEY : getShaderKey(vertexShader, fragmentShader);
  if (resources.find(key) == resources.end()) {
    resources[key] = newShader(vertexShader, fragmentShader, key);
  }

  return resource<shaderProgram>((shaderProgram*) resources[key], this);
}

// Find the default shader program, if it is set
resource<shaderProgram> resourceHandler::loadShaderProg() {
  if (resources.find(SHADER_KEY) == resources.end()) {
    recordLog("ERROR: Could not find the main shader. Are you sure it loaded?");
    return resource<shaderProgram>(nullptr, this);
  }

  return resource<shaderProgram>((shaderProgram*) resources[SHADER_KEY], this);
}

// Find the named light, if it is set.
resource<light> resourceHandler::loadLight(const string& name) {
  map<string, raw_resource*>::iterator it = resources.find(name);
  if (it == resources.end()) {
    recordLog("Loading light " + name);
    resources[name] = new light(name, glm::vec4(0.0, 0.0, 0.0, 1.0), glm::vec3(1.0, 1.0, 1.0));
    lights[name] = (light*) resources[name];
  }
	
  return resource<light>((light*) resources[name], this);
}

// Find the named camera, if it exists.
resource<camera> resourceHandler::loadCamera(const string& name) {  
  map<string, raw_resource*>::iterator it = resources.find(name);
  if (it == resources.end()) {
    recordLog("Loading camera " + name);
    resources[name] = new camera(name);
    cameras[name] = (camera*) resources[name];
  }

  return resource<camera>((camera*) resources[name], this);
}

// Find the named fp camera, if it exists.
resource<firstPersonCamera> resourceHandler::loadFirstPersonCamera(const string& name) {
  map<string, raw_resource*>::iterator it = resources.find(name);
  if (it == resources.end()) {
    recordLog("Loading camera " + name);
    resources[name] = new firstPersonCamera(name);
    cameras[name] = (camera*) resources[name];
  }

  return resource<firstPersonCamera>((firstPersonCamera*) resources[name], this);
}

// Find the named tb camera, if it exists
resource<trackBallCamera> resourceHandler::loadTrackBallCamera(const string& name) {
  map<string, raw_resource*>::iterator it = resources.find(name);
  if (it == resources.end()) {
    recordLog("Loading camera " + name);
    resources[name] = new trackBallCamera(name);
    cameras[name] = (camera*) resources[name];
  }

  return resource<trackBallCamera>((trackBallCamera*) resources[name], this);
}

resource<skybox> resourceHandler::loadSkybox(const string& path, const string& extension) {
  map<string, raw_resource*>::iterator it = resources.find(path);
  if (it == resources.end()) {
    recordLog("Loading skybox " + path);
    
    string skyboxTextures[NUM_SKYBOX_TEXTURES];
    skyboxTextures[SKYBOX_RIGHT]  = (path + "/right." + extension);
    skyboxTextures[SKYBOX_LEFT]   = (path + "/left." + extension);
    skyboxTextures[SKYBOX_TOP]    = (path + "/top." + extension);
    skyboxTextures[SKYBOX_BOTTOM] = (path + "/bottom." + extension);
    skyboxTextures[SKYBOX_BACK]   = (path + "/back." + extension);
    skyboxTextures[SKYBOX_FRONT]  = (path + "/front." + extension);

    string vertexShader = getDataFolderPath("shaders/skybox.v.glsl");
    string fragShader = getDataFolderPath("shaders/skybox.f.glsl");
    string shaderKey = getShaderKey(vertexShader, fragShader);
    if (resources.find(shaderKey) == resources.end()) {
      resources[shaderKey] = newShader(vertexShader, fragShader, shaderKey);
    }

    skybox* newSkybox = new skybox(path);
    newSkybox->setShaderProgram((shaderProgram*)resources[shaderKey]);
    newSkybox->setTextures(skyboxTextures);
    newSkybox->setActiveCamera(cameras[activeCameraID]);
    resources[path] = newSkybox;
  }

  return resource<skybox>((skybox*) resources[path], this);
}

// Unload the named resource
void resourceHandler::unload(const string& name) {
  map<string, raw_resource*>::iterator it = resources.find(name);
  if (it != resources.end()) {
    recordLog("Unloading resource " + name);
    delete it->second;
    resources.erase(it);
    lights.erase(name);
    cameras.erase(name);
  }
  else {
    recordLog("WARNING: Could not find resource " + name + " for unloading.");
  }
}

// Remove all resources
void resourceHandler::unloadAll() {
  for (auto p : resources) 
    delete p.second;
  resources.clear();

  for (auto p : cameras)
    delete p.second;
  cameras.clear();
  
  for (auto p : lights)
    delete p.second;
  lights.clear();
}

// Get a list of all lights in the scene
vector<light*> resourceHandler::getAllLights() const {
  vector<light*> lightList;
  for (auto p : lights) {
    lightList.push_back(p.second);
  }

  return lightList;
}

// Set the active camera
void resourceHandler::setActiveCamera(const string& id) {
  activeCameraID = id;
}

// Get the active camera
camera* resourceHandler::getActiveCamera() const {
  if (cameras.find(activeCameraID) == cameras.end()) {
    recordLog("WARNING: Could not find camera " + activeCameraID);
    return nullptr;
  }
  
  return cameras.at(activeCameraID);
}
