#version 330 core
layout (location = 0) in vec4 position;
layout (location = 1) in vec4 color;

uniform mat4 transformMatrix;

smooth out vec4 theColor;

void main(void) {
     gl_Position = transformMatrix * position;
     theColor = color;
}
