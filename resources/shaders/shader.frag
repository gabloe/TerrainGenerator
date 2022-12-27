#version 330 core

in vec3 FragPos;
in vec3 FragColor;
in vec3 LightPosition;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 camera;

// output
out vec4 OutColor;

// white light
vec3 lightColor = vec3(1.0, 1.0, 1.0);

// Object color
vec4 objectColor = vec4(1.0, 1.0, 1.0, 1.0);

struct Material {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
    float shininess_strength;
}; 

uniform Material material;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_height1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;

void main(void)
{
    vec3 specular_tex = texture(texture_specular1, TexCoords).rgb;
    vec3 diffuse_tex = texture(texture_diffuse1, TexCoords).rgb;
    vec3 normal_tex = texture(texture_normal1, TexCoords).rgb;
    vec3 height_tex = texture(texture_height1, TexCoords).rgb;

    vec3 norm = normalize(Normal * 2.0 - 1.0);
    vec3 lightDir = normalize(LightPosition - FragPos);
    vec3 viewDir = normalize(camera - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    // Ambient
    vec3 ambient = material.ambient.xyz * lightColor;

    // Diffuse
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lightColor * diff * material.diffuse.xyz;

    // Specular
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = material.shininess_strength * spec * material.specular.xyz * lightColor;

    // Result
    OutColor = vec4(ambient + diffuse + specular, 1.0) * objectColor;
}
