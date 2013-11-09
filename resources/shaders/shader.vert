#version 150 // Specify which version of GLSL we are using.
// in_Position was bound to attribute index 0("shaderAttribute")
in  vec3 in_Position;
uniform mat4 rotation;
int main(void)
{
    gl_Position = vec4(in_Position.x, in_Position.y, in_Position.z, 1.0) * rotation;
    return 0;
}