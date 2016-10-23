#version 410 core

#define POINT_SIZE      0.1f

layout (location = 0) in vec4 pos;

//TODO use uniform buffer object because

uniform mat4 mvp;

out vec4 color;

void main() {
    gl_Position = mvp * pos;
    gl_PointSize = max(POINT_SIZE, POINT_SIZE / gl_Position.w);
    color = gl_Position;
}
