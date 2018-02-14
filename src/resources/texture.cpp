#include "texture.hpp"

using namespace std;

texture::texture(const string& name) : raw_resource(name) {
    texID = SOIL_load_OGL_texture(name.c_str(),
        SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
}

texture::~texture() {
    glDeleteTextures(1, &texID);
}

void texture::draw(GLuint index) {
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D, texID); 
}