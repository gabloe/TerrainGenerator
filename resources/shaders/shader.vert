#version 410
in vec3 in_Position;
//out vec4 v_Position;
uniform mat4 transform,projection;
void main(void)
{
    //v_Position = vec4(normalize(in_Position),1);
    gl_Position = projection * transform * vec4(in_Position.x, in_Position.y, in_Position.z, 1.0);
}
