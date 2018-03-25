#include "glTexture.hpp"
#include "log.hpp"

using namespace std;

glTexture::~glTexture() {
    glDeleteTextures(1, &texID);
}

bool glTexture::loadTexture() {
    texID = SOIL_load_OGL_texture(name.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
    return (texID != 0);
}

void glTexture::draw(GLuint index) {
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D, texID);
}