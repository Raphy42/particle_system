#version 410 core

layout (location = 0) in vec4 pos;
layout (location = 1) in vec4 vel;

//TODO use uniform buffer object because

uniform mat4 mvp;

out vec4 speed;

void main() {
    gl_Position = mvp * pos;
    gl_PointSize = gl_Position.z;
    gl_PointSize = pos.w;
    speed = vel;
}
