#version 150
in vec3 v_Position;
out vec4 gl_Postion;

uniform mat4 transform,projection;
void main(void)
{
    gl_Position = vec4(v_Position, 1.0); //projection * transform * vec4(v_pos, 1.0);
}
