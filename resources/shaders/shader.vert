#version 150

in vec3 v_Position;
in vec3 v_Normal;

uniform mat4 projection,translate;
uniform vec3 camera;

out vec3 norm;
out vec3 vect;

void main(void) {
	vect = (translate*vec4(v_Position, 1.0)).xyz;
    norm = normalize(v_Normal - camera);

    gl_Position = projection*vec4(vect,1.0);
}
