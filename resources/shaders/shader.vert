#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec2 aTexCoords;
layout (location = 4) in vec3 aTangent;
layout (location = 5) in vec3 aBitangent;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

out vec3 FragPos;
out vec3 FragColor;
out vec3 LightPosition;
out vec3 Normal;
out vec2 TexCoords;

void main(void)
{
    FragPos = vec3(model * vec4(aPos, 1.0));

    // These will not affect the location of the vertices
    LightPosition       = vec3(model * view * vec4(10.0f, 10.0f, 0.0f, 0.0)); // TODO: This should be a uniform
    Normal              = mat3(transpose(inverse(model))) * aNormal;
    FragColor           = aColor;
    TexCoords           = aTexCoords;

    gl_Position         = projection * view * model * vec4(aPos, 1.0);
}
