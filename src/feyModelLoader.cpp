#include "feyModelLoader.hpp"

#include <fstream>
#include <map>
#include <cstdlib>
#include <string>

#include "paths.hpp"
#include "log.hpp"
#include "glHeaders.hpp"

using namespace std;

model* loadFeyModel(string filename) {
	recordLog("Reading fey model " + filename + "...");
  model* m = NULL;
  ifstream fin(filename.c_str());

  if (fin.is_open()) {
    int numVertices = 0;
    fin >> numVertices;

    vector<glm::vec3> vertexList;
    for (int i = 0; i < numVertices; i++) {
      float x, y, z;

      fin >> x;
      fin >> y;
      fin >> z;

      vertexList.push_back(glm::vec3(x, y, z));
    }
	
	m = new model();
	
	int numTextures = 0;
	fin >> numTextures;
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
		
		m->setTexture(getFullPath("feyData/library/" + filename));
	}

	int numVerts = 0;
	fin >> numVerts;
	numVerts /= 2;
	
    vector<glm::vec2> uvMapping;
	vector<glm::vec3> finalVerts;
    for(int i = 0; i < numVerts; i++) {
		int index, uvIndex;
		
		fin >> index;
		finalVerts.push_back(vertexList[index]);
		
		fin >> uvIndex;
		uvMapping.push_back(uvCoords[uvIndex]);
    }

    m->setVertices(finalVerts);
	m->setUVMapping(uvMapping);
	recordLog("Successfully read in fey model file " + filename + "!");
  }

  else {
    recordLog("Could not read file: " + filename);
  }

  return m;
}
