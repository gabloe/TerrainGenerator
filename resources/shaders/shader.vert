#version 150
in vec3 v_Position;

uniform mat4 projection;
void main(void)
{
    gl_Position = projection * vec4(v_Position, 1.0);
}
