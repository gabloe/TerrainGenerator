#version 150

out vec4 colorOut;

in vec3 norm;
in vec3 eye;

void main(void) {
	float intensity = -dot(eye , norm);
	if(intensity < 0.0) intensity = 0.0;
    colorOut =intensity * vec4(1.0, 0.0, 0.0, 1.0); // max(intensity * diffuse + spec, ambient);
}
