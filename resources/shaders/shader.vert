#version 150

in vec3 v_Position;
in vec3 v_Normal;
uniform mat4 projection,translate;

out vec3 norm;

void main(void) {
	norm = v_Normal;
    gl_Position = projection*translate*vec4(v_Position, 1.0);
}
