#version 150

in vec3 norm;

out vec4 colorOut;

void main(void) {
    colorOut= vec4(abs(norm.x),abs(norm.y),abs(norm.z), 1.0);
}
