#version 330 core
layout (location = 0) in vec3 vertexPosition;

out vec4 vertexColor;

void main() {
    gl_Position = vec4(vertexPosition, 1.0);
    vertexColor = vec4(0.5f, 0.66f, 1.0f, 1.0f);
}