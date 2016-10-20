#version 410 core

in vec4 color;

out vec4 colour;

void main() {
    if (gl_PrimitiveID % 4 == 0) {
        colour = vec4(0.2);
    }
    else if (gl_PrimitiveID % 3 == 0) {
        colour = vec4(0.8);
    }
    else if (gl_PrimitiveID % 2 == 0) {
        colour = vec4(0.6);
    }
    else {
        colour = vec4(0.4);
    }
    colour *= color;
}
