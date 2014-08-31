#version 150

in vec3 v_Position;

out vec4 result;

uniform mat4 projection,translate;

void main(void) {
    result = projection*translate*vec4(v_Position, 1.0);
}
