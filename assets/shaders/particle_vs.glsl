#version 410 core

layout (location = 0) in vec4 pos;
layout (location = 1) in vec4 vel;
layout (location = 2) in vec4 colour;

//TODO use uniform buffer object because

uniform mat4 mvp;
uniform vec4 center;
uniform float delta_time;

out vec4 color_cl;
out vec4 o_center;
out float delta_t;

void main() {
    gl_Position = mvp * pos;
    gl_PointSize = gl_Position.z;
    gl_PointSize = pos.w;
    color_cl = colour;
    o_center = pos - center;
    delta_t = delta_time;
}
