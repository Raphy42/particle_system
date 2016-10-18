#version 410 core

layout (location = 0) in vec4 pos;

//uniform mat4 mvp;

out vec4 color;

void main() {
    gl_Position = pos;
    gl_PointSize = max(1.f, 1.f / gl_Position.w);
    color = gl_Position;
}
