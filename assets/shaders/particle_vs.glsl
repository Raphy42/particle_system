#version 410 core

layout (location = 0) in vec4 pos;
layout (location = 1) in vec4 vel;

//TODO use uniform buffer object because

uniform mat4 mvp;
uniform vec4 center;

out vec4 speed;
out vec4 o_center;

void main() {
    gl_Position = mvp * pos;
    gl_PointSize = gl_Position.z;
    gl_PointSize = pos.w;
    speed = vel;
    o_center = center;
}
