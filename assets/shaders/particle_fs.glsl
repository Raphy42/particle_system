#version 410 core

in vec4 speed;

out vec4 colour;

void main() {
	colour = normalize(speed) / 2.f;
}
