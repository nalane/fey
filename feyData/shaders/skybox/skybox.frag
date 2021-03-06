#version 450
#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec3 TexCoords;
layout (location = 0) out vec4 color;

layout (binding = 1) uniform samplerCube skybox;

void main() {    
    color = texture(skybox, TexCoords);
}