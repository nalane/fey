#include "glTerrain.hpp"
#include "glShaderProgram.hpp"
#include "glTexture.hpp"
#include "log.hpp"

#include <cmath>

using namespace std;

glTerrain::glTerrain(const string& name) : terrain(name) {
    bindingPoint = uniformBindingPoint++;
}

glTerrain::~glTerrain() {
    uniformBindingPoint--;

    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ubo);
    glDeleteVertexArrays(1, &vao);
}

void glTerrain::bindData() {
    glShaderProgram* prog = (glShaderProgram*)child_resources["shaderProgs"]["default"];
    prog->useProgram();

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    size_t dataSize = vertices.size() * sizeof(terrainVertex);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, dataSize, vertices.data(), GL_STATIC_DRAW);

    terrainVertex::bindLayout();

    // Bind uniforms to shader
    glGenBuffers(1, &ubo);
    GLuint uniformIndex = glGetUniformBlockIndex(prog->getProgID(), "TerrainUniforms");
    glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, ubo);
    glUniformBlockBinding(prog->getProgID(), uniformIndex, bindingPoint);
    size_t muSize = sizeof(modelUniforms);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(modelUniforms), nullptr, GL_DYNAMIC_DRAW);
}

void glTerrain::draw(modelUniforms uniforms) {
    glShaderProgram* prog = (glShaderProgram*)child_resources["shaderProgs"]["default"];
    prog->useProgram();

    glBindVertexArray(vao);
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(uniforms), &uniforms);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    if (child_resources["textures"].size() > 0) {
        // Bind texture to uniform
        int index = 0;
        GLuint texHandle = glGetUniformLocation(prog->getProgID(), "texSampler");
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texHandle);
        glUniform1i(texHandle, 0);

        // Draw textures
        for (auto p : child_resources["textures"]) {
            glTexture* tex = (glTexture*)(p.second);
            tex->draw(index);
            index++;
        }
    }

    // Draw patches
    const int patchSideLength = round(sqrt(PATCH_SIZE)) - 1;
    const int numQuads = patchSideLength * patchSideLength;
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glPatchParameteri(GL_PATCH_VERTICES, PATCH_SIZE);
    glDrawArrays(GL_PATCHES, 0, vertices.size());
}