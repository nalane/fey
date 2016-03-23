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
	recordLog("Reading in " + filename + "...");
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
	for (int i = 0; i < numTextures; i++) {
		string filename = "junk";
		int numUVCoords = -1;
		
		getline(fin, filename);
		getline(fin, filename);
		fin >> numUVCoords;
		
		vector<glm::vec2> uvMap;
		for (int j = 0; j < numUVCoords; j++) {
			int uvIdx;
			float u, v;
			
			fin >> uvIdx;
			fin >> u;
			fin >> v;
			
			uvMap.push_back(glm::vec2(u, v));
		}
		
		m->setTexture(getFullPath("feyData/library/" + filename), uvMap);
	}

	int numVerts = 0;
	fin >> numVerts;
	numVerts /= 2;
	
    vector<int> indexList, uvIndexList;
    for(int i = 0; i < numVerts; i++) {
		int index, uvIndex;
		
		fin >> index;
		indexList.push_back(index);
		
		fin >> uvIndex;
		uvIndexList.push_back(uvIndex);
    }

    m->setVertices(vertexList);
    m->setElementIndices(indexList);
	recordLog("Successfully read in fey model file " + filename + "!");
  }

  else {
    recordLog("Could not read file: " + filename);
  }

  return m;
}
