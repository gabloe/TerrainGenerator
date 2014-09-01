#version 150

out vec4 colorOut;

in vec3 color;

void main(void) {
	colorOut = vec4(color,1.0);
}
