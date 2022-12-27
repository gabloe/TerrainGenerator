#version 330 core

in vec3 FragPos;
in vec3 FragColor;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 camera;

// output
out vec4 OutColor;

struct Material {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
    float shininess_strength;
}; 
uniform Material material;

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
#define NR_DIR_LIGHTS 2
uniform DirLight dirLight[NR_DIR_LIGHTS];

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
#define NR_POINT_LIGHTS 2
uniform PointLight pointLights[NR_POINT_LIGHTS];

// Prototypes
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results (NOTE: At some point replace material color with material textures)
    vec3 ambient  = light.ambient * material.ambient.xyz; // * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * material.diffuse.xyz; //* vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * material.specular.xyz; //* vec3(texture(material.specular, TexCoords));
    return (ambient + diffuse + specular);
}

// calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient = light.ambient * material.ambient.xyz; // * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * material.diffuse.xyz; //* vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * material.specular.xyz; //* vec3(texture(material.specular, TexCoords));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

void main(void)
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(camera - FragPos);
    vec3 result = vec3(0.0, 0.0, 0.0);

    // phase 1: directional lights
    for(int i = 0; i < NR_DIR_LIGHTS; i++)
        result += CalcDirLight(dirLight[i], norm, viewDir);
    // phase 2: point lights
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);  

    // Result
    OutColor = vec4(result, 1.0);
}
