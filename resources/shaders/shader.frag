#version 150

out vec4 colorOut;

in vec3 norm;

void main(void) {
	vec3 n = normalize(norm);
    colorOut = vec4(n,1.0); // vec4(1.0, 0.0, 0.0, 1.0); // max(intensity * diffuse + spec, ambient);
}
