#version 150
in vec3 v_Position;

out vec4 pos;
uniform mat4 projection;
void main(void)
{
    pos = projection * vec4(v_Position, 1.0);
}
