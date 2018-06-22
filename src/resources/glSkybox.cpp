#include "glSkybox.hpp"
#include "glShaderProgram.hpp"
#include "skyboxUniforms.hpp"
#include "camera.hpp"
#include "scene.hpp"
#include "log.hpp"

using namespace std;

glSkybox::glSkybox(const string& name) : skybox(name) {
    bindingPoint = uniformBindingPoint++;
}

glSkybox::~glSkybox() {
    uniformBindingPoint--;

    glDeleteTextures(1, &texID);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ubo);
    glDeleteVertexArrays(1, &vao);
}

void glSkybox::setTextures(texture* tex, string texturePaths[NUM_SKYBOX_TEXTURES]) {
    glShaderProgram* prog = (glShaderProgram*)child_resources["shaderProgs"]["default"];
    prog->useProgram();

    // Generate the vao
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Generate and fill the vbo
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 4 * vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
  
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
        unsigned char* imageData = SOIL_load_image(texturePaths[i].c_str(),
            &width, &height, &channels, SOIL_LOAD_RGB);
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

void glSkybox::bindData() {
    glShaderProgram* prog = (glShaderProgram*)child_resources["shaderProgs"]["default"];
    prog->useProgram();

    glBindVertexArray(vao);

    skyboxVertex::bindLayout();

    // Bind uniforms to shader
    glGenBuffers(1, &ubo);
    GLuint uniformIndex = glGetUniformBlockIndex(prog->getProgID(), "SkyboxUniforms");
    glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, ubo);
    glUniformBlockBinding(prog->getProgID(), uniformIndex, bindingPoint);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(skyboxUniforms), nullptr, GL_DYNAMIC_DRAW);
}

void glSkybox::draw() {
    // Use the skybox shader prog
    glShaderProgram* prog = (glShaderProgram*)(child_resources["shaderProgs"]["default"]);
    prog->useProgram();
    glBindVertexArray(vao);
  
    // Move the view matrices to the GPU
    skyboxUniforms uniforms;
    camera* activeCamera = scene::getActiveScene()->getActiveCamera();
    glm::mat4 viewMatrix = glm::mat4(glm::mat3(activeCamera->getViewMatrix()));
    uniforms.vpMatrix = activeCamera->getProjectionMatrix() * viewMatrix;
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(uniforms), &uniforms);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
  
    // Draw the skybox
    glDepthMask(GL_FALSE);
    glEnableVertexAttribArray(0);

    GLuint texHandle = glGetUniformLocation(prog->getProgID(), "skybox");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texHandle);
    glUniform1i(texHandle, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texID);
    
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthMask(GL_TRUE);
}