#version 410 core

in vec4 speed;
in vec4 o_center;

out vec4 colour;


void main() {
	colour =  normalize(speed);
	colour = 1 - colour;
}
