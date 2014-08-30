#version 150
in vec3 in_Position;
out vec4 gl_Position;
uniform mat4 transform,projection;
void main(void)
{
    gl_Position = projection * transform * vec4(in_Position.x, in_Position.y, in_Position.z, 1.0);
}
