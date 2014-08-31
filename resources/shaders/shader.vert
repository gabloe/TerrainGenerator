#version 150

in vec3 v_Position;
in vec3 v_Normal;

uniform mat4 projection,translate;
uniform vec3 camera;

out vec3 norm;
out vec3 eye;

void main(void) {
	norm = normalize(translate*vec4(v_Normal,1.0)).xyz;
	eye = normalize(camera);
	gl_Position = projection * translate * vec4(v_Position, 1.0);
}
