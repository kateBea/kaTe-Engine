#version 410 core
out vec4 pixelColor;
in vec3 v_Color;

void main() {
    pixelColor = vec4(v_Color * 0.5f + 0.5f, 1.0f);
}