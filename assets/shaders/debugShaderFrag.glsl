#version 330 core
out vec4 pixelColor;

in vec4 vertexColor;

void main() {
    pixelColor = vertexColor;
}