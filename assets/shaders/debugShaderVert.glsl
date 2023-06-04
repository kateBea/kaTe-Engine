#version 410 core
layout (location = 0) in vec3 a_Position;

out vec3 v_Color;
void main() {
    v_Color = a_Position;
    gl_Position = vec4(a_Position, 1.0);
}