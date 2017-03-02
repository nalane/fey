#include "resourceHandler.hpp"

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
model* resourceHandler::loadFeyModel(string filename) {
  string fullPath = getLibraryFolderPath(filename);
  recordLog("Reading fey model " + fullPath + "...");
  model* m = NULL;
  ifstream fin(fullPath.c_str());

  if (fin.is_open()) {
    m = new model(filename);
    
    // Get number of materials
    int numMaterials = 0;
    fin >> numMaterials;

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
    numVerts /= 2;
		
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
      uvMapping.push_back(uvCoords[uvIndex]);
	  
	  face[i % 3] = vertexList[index];
	  if (i % 3 == 2) {
		  glm::vec3 normal = glm::cross(face[1] - face[0], face[2] - face[0]);
		  normals.push_back(normal);
		  normals.push_back(normal);
		  normals.push_back(normal);
	  }
    }

    // Push data into the model
    m->setVertices(finalVerts);
    m->setUVMapping(uvMapping);
    m->setNormals(normals);
    recordLog("Successfully read in fey model file " + filename + "!");
  }

  else {
    recordLog("Could not read file: " + filename);
  }

  return m;
}

// Get the model associated with the given filename
resource<model> resourceHandler::loadModel(string filepath) {
  if (resources.find(filepath) == resources.end())
    resources[filepath] = loadFeyModel(filepath);

  return resource<model>((model*) resources[filepath], this);
}

// Get a vertex shader
shader* resourceHandler::loadVertexShader(string vertexShaderPath) {
  if (resources.find(vertexShaderPath) == resources.end())
    resources[vertexShaderPath] = new shader(vertexShaderPath, GL_VERTEX_SHADER);

  return (shader*) resources[vertexShaderPath];
}

// Get a fragment shader
shader* resourceHandler::loadFragmentShader(string fragmentShaderPath) {
  if (resources.find(fragmentShaderPath) == resources.end())
    resources[fragmentShaderPath] = new shader(fragmentShaderPath, GL_FRAGMENT_SHADER);

  return (shader*) resources[fragmentShaderPath];
}

// Get a shader program
resource<shaderProgram> resourceHandler::loadShaderProg(string vertexShader, string fragmentShader, bool defaultShader) {
  string key = defaultShader ? SHADER_KEY : "v" + vertexShader + "f" + fragmentShader;
  if (resources.find(key) == resources.end()) {
    shaderProgram* prog = new shaderProgram(key);
    prog->addShader(loadVertexShader(vertexShader));
    prog->addShader(loadFragmentShader(fragmentShader));
    prog->loadShaders();
    prog->compileShaders();
    prog->linkShaders();

    resources[key] = prog;
  }

  return resource<shaderProgram>((shaderProgram*) resources[key], this);
}

// Find the default shader program, if it is set
resource<shaderProgram> resourceHandler::loadShaderProg() {
  if (resources.find(SHADER_KEY) == resources.end()) {
    recordLog("ERROR: Could not find the main shader. Are you sure it loaded?");
    return resource<shaderProgram>(NULL, this);
  }

  return resource<shaderProgram>((shaderProgram*) resources[SHADER_KEY], this);
}

void resourceHandler::unload(string name) {
  map<string, raw_resource*>::iterator it = resources.find(name);
  if (it != resources.end()) {
    recordLog("Unloading resource " + name);
    delete it->second;
    resources.erase(it);
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

// Add a named light
void resourceHandler::setLight(string id, light* l) {
  lights[id] = l;
}

// Get a light based on its id
light* resourceHandler::getLight(string id) {
  return lights[id];
}

// Get a list of all lights in the scene
vector<light*> resourceHandler::getAllLights() {
  vector<light*> lightList;
  for (auto p : lights) {
    lightList.push_back(p.second);
  }

  return lightList;
}

// Removes all lights from a scene
void resourceHandler::removeLights() {
	for (auto p : lights) {
		delete p.second;
	}
	lights.clear();
}

// Adds a camera to the register
void resourceHandler::setCamera(std::string id, camera* cam) {
  cameras[id] = cam;
}

// Find a camera based on its id
camera* resourceHandler::getCamera(std::string id) {
  return cameras[id];
}

// Removes all cameras from a scene
void resourceHandler::removeCameras() {
	for (auto p : cameras) {
		delete p.second;
	}
	cameras.clear();
}

// Set the active camera
void resourceHandler::setActiveCamera(string id) {
  activeCameraID = id;
}

// Get the active camera
camera* resourceHandler::getActiveCamera() {
  if (cameras.find(activeCameraID) == cameras.end()) {
    recordLog("WARNING: Could not find camera " + activeCameraID);
    return NULL;
  }
  
  return cameras[activeCameraID];
}
