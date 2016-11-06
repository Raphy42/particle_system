#version 410 core

layout (location = 0) in vec4 pos;

//TODO use uniform buffer object because

uniform mat4 mvp;

out vec4 color;

void main() {
    gl_Position = mvp * pos;
    gl_PointSize = gl_Position.z;
    color = gl_Position;
}
