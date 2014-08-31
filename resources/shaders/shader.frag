#version 150

in vec3 norm;

out vec4 colorOut;

void main(void) {
	float derp = dot(vec4(norm,1.0) , vec4(1.0,0.0,0.0,1.0));
    colorOut= derp * vec4(1.0, 0.0, 0.0, 1.0);
}
