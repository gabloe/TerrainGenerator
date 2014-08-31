#version 150

in vec3 norm;
in vec3 vect;

out vec4 colorOut;

void main(void) {
    colorOut=  max(0.0, dot(norm,vect)) * vec4(1.0,0.0,0.0, 1.0);
}
