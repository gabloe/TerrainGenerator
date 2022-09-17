#version 330 core

in vec4 fPosition;
in vec4 fColor;
in vec4 fLightPosition;
in vec3 fNormal;

uniform vec3 camera;

// output
out vec4 color;

// white light
vec3 lightColor = vec3(1.0, 1.0, 1.0);

struct Material {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
    float shininess_strength;
}; 
  
uniform Material material;

void main(void)
{   
    // Ambient
    vec3 ambient = material.ambient.xyz * lightColor;

    // Diffuse
    vec3 norm = normalize(fNormal);
    vec3 lightDir = -normalize(fLightPosition.xyz + fPosition.xyz);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lightColor * (diff * material.diffuse.xyz);

    // Specular
    vec3 viewDir = normalize(camera - fPosition.xyz);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = lightColor * (spec * material.specular.xyz) * material.shininess_strength;

    // Result
    vec4 result = vec4(ambient + diffuse + specular, 1.0);

    color = result;
}
