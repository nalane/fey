#include "log.hpp"
#include "model.hpp"

using namespace std;

// Sets up the vao
model::model() {
  glGenVertexArrays(1, &vao);
  nonPassIndex = -2;
  elementsIndex = 0;
}

// Removes all vbos and the vao
model::~model() {
  glDeleteVertexArrays(1, &vao);
  for (pair<int, GLuint> p : vbos)
    glDeleteBuffers(1, &(p.second));
}

// Adds a VBO
void model::addData(GLenum target, GLsizeiptr size, void* data, GLenum usage, int shaderLocation) {
  if (shaderLocation < 0) {
    shaderLocation = nonPassIndex;
    nonPassIndex--;
  }
	
  if (target == GL_ELEMENT_ARRAY_BUFFER) {
    elementsIndex = shaderLocation;
  }
	
  glBindVertexArray(vao);
	
  vbos[shaderLocation] = -1;
  glGenBuffers(1, &(vbos[shaderLocation]));
	
  glBindBuffer(target, vbos[shaderLocation]);
  glBufferData(target, size, data, usage);
  
  if (shaderLocation >= 0) {
    glEnableVertexAttribArray(shaderLocation);
    glVertexAttribPointer(shaderLocation, 4, GL_FLOAT, GL_FALSE, 0, 0);
  }
}

// Sends vertex data to the GPU
void model::setVertices(vector<glm::vec3> vertexList) {
  numVertices = vertexList.size();
  vector<float> rawData;
  for (glm::vec3 v : vertexList) {
    rawData.push_back(v.x);
    rawData.push_back(v.y);
    rawData.push_back(v.z);
    rawData.push_back(1.0);
  }
  addData(GL_ARRAY_BUFFER, rawData.size() * sizeof(float), &rawData[0], GL_STATIC_DRAW, 0);
}

// Sends vertex color data to GPU
void model::setColors(std::vector<glm::vec4> colorList) {
  vector<float> rawData;
  for (glm::vec4 c : colorList) {
    rawData.push_back(c.r);
    rawData.push_back(c.g);
    rawData.push_back(c.b);
    rawData.push_back(c.a);
  }
  addData(GL_ARRAY_BUFFER, rawData.size() * sizeof(float), &rawData[0], GL_STATIC_DRAW, 1);
}

// Sets the drawing order for vertices
void model::setElementIndices(vector<int> indexList) {
  if (elementsIndex == 0) {
    addData(GL_ELEMENT_ARRAY_BUFFER, indexList.size() * sizeof(int), &indexList[0], GL_STATIC_DRAW);
    elementsSize = indexList.size();
  }
	
  else {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[elementsIndex]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexList.size() * sizeof(int), &indexList[0], GL_STATIC_DRAW);
    elementsSize = indexList.size();
  }
}

// Draws the model
void model::draw() {
  glBindVertexArray(vao);
	
  if (elementsIndex < 0) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[elementsIndex]);
    glDrawElements(GL_TRIANGLES, elementsSize, GL_UNSIGNED_INT, NULL);
  }
  
  else {
    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
    glDrawArrays(GL_TRIANGLES, 0, numVertices);
  }
}