#version 330 core
layout (location = 0) in vec3 position;

uniform mat4 vpMatrix;

out vec3 TexCoords;

void main() {
    gl_Position = vpMatrix * vec4(position, 1.0);  
    TexCoords = position;
}  