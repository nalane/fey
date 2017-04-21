#include "skybox.hpp"
#include "resourceHandler.hpp"
#include "log.hpp"

using namespace std;

#define SIZE 333.3f

// Remove data from GPU buffer
skybox::~skybox() {
  glDeleteVertexArrays(1, &vao);
  glDeleteTextures(1, &texID);
}

// Create the skybox textures
void skybox::setTextures(string texturePaths[NUM_SKYBOX_TEXTURES]) {
  // Generate vertices
  float data[] = {
    // Positions
	-SIZE,  SIZE, -SIZE,
    -SIZE, -SIZE, -SIZE,
    SIZE, -SIZE, -SIZE,
    SIZE, -SIZE, -SIZE,
    SIZE,  SIZE, -SIZE,
    -SIZE,  SIZE, -SIZE,
  
    -SIZE, -SIZE,  SIZE,
    -SIZE, -SIZE, -SIZE,
    -SIZE,  SIZE, -SIZE,
    -SIZE,  SIZE, -SIZE,
    -SIZE,  SIZE,  SIZE,
    -SIZE, -SIZE,  SIZE,
  
    SIZE, -SIZE, -SIZE,
    SIZE, -SIZE,  SIZE,
    SIZE,  SIZE,  SIZE,
    SIZE,  SIZE,  SIZE,
    SIZE,  SIZE, -SIZE,
    SIZE, -SIZE, -SIZE,
   
    -SIZE, -SIZE,  SIZE,
    -SIZE,  SIZE,  SIZE,
    SIZE,  SIZE,  SIZE,
    SIZE,  SIZE,  SIZE,
    SIZE, -SIZE,  SIZE,
    -SIZE, -SIZE,  SIZE,
  
    -SIZE,  SIZE, -SIZE,
    SIZE,  SIZE, -SIZE,
    SIZE,  SIZE,  SIZE,
    SIZE,  SIZE,  SIZE,
    -SIZE,  SIZE,  SIZE,
    -SIZE,  SIZE, -SIZE,
  
    -SIZE, -SIZE, -SIZE,
    -SIZE, -SIZE,  SIZE,
    SIZE, -SIZE, -SIZE,
    SIZE, -SIZE, -SIZE,
    -SIZE, -SIZE,  SIZE,
    SIZE, -SIZE,  SIZE
  };
  
  // Generate and fill the vbo
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, 3 * 36 * sizeof(float), &data, GL_STATIC_DRAW);
  
  // Generate and fill the vao
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  
  // Generate texture
  glActiveTexture(GL_TEXTURE0);
  glGenTextures(1, &texID);
  glBindTexture(GL_TEXTURE_CUBE_MAP, texID);

  // Load images to texture
  for (int i = 0; i < NUM_SKYBOX_TEXTURES; i++) {
    int width;
    int height;
	int channels;
	
	// Load image to memory
    unsigned char* imageData = SOIL_load_image(texturePaths[i].c_str(), &width, &height, &channels, SOIL_LOAD_RGB);
	if (imageData == nullptr) {
		recordLog("WARNING: Could not load " + texturePaths[i] + "!");
	}
	
	// Move image to GPU memory
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB,
		 width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
	delete imageData;
  }

  // Set drawing methods
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); 
}

void skybox::draw() {
	// Use the skybox shader prog
  shaderProgram* prog = (shaderProgram*)(child_resources["shaderProg"]);
  prog->useProgram();
  
  // Move the view matrices to the GPU
  glm::mat4 viewMatrix = glm::mat4(glm::mat3(activeCamera->getViewMatrix()));
  glm::mat4 vpMatrix = activeCamera->getProjectionMatrix() * viewMatrix;
  GLint vpHandle = glGetUniformLocation(prog->getProgID(), "vpMatrix");
  glUniformMatrix4fv(vpHandle, 1, GL_FALSE, &vpMatrix[0][0]);
  
  // Draw the skybox
  glDepthMask(GL_FALSE);
  glBindVertexArray(vao);
  glEnableVertexAttribArray(0);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, texID);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glDepthMask(GL_TRUE);
}
