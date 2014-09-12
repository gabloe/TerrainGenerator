#version 150

bool isEqual(in vec3 a, in vec3 b) {
	if( abs(a.x - b.x) > 0.00001 ) return false;
	return abs(a.z - b.z) < 0.00001;
}


out vec4 colorOut;

const vec4 ambient = vec4(0.2,0.2,0.2,1.0);

const float shininess = 0.2;

in vec3 N;
in vec3 v;

uniform vec3 p1,p2,p3;

out vec4 color;

struct lightSource
   {
   vec4 position;
   vec4 diffuse;
   vec4 specular;
   float constantAttenuation, linearAttenuation, quadraticAttenuation;
   float spotCutoff, spotExponent;
   vec3 spotDirection;
   };

struct material
   {
   vec4 ambient;
   vec4 diffuse;
   vec4 specular;
   float shininess;
   };

material frontMaterial = material(
  vec4(0.2, 0.2, 0.2, 1.0),
  vec4(1.0, 0.8, 0.8, 1.0),
  vec4(1.0, 1.0, 1.0, 1.0),
  5.0
);

const int numLights = 1;
lightSource lights[numLights];

lightSource sun = lightSource(
   vec4(0.0,10.0,0.0,0.0),
   vec4(1.0,1.0,1.0,1.0),
   vec4(0.1,0.1,0.1,1.0),
   0.0, 1.0, 0.0,
   180.0, 0.0,
   vec3(0.0,0.0,0.0)
);

void main(void) {
        lights[0] = sun;

        vec3 normalDir = normalize(N);
	vec3 viewDir = normalize(v);
        vec3 lightDir;
        float attenuation;

        vec3 totalLighting = vec3(ambient) * vec3(frontMaterial.ambient);

        for (int i=0; i<numLights; i++)
           {
           if (lights[i].position.w == 0.0)
              {
              attenuation = 1.0;
              lightDir = normalize(vec3(lights[i].position));
              }
           else
              {
              vec3 positionToLight = vec3(lights[i].position - vec4(v,1.0));
              float distance = length(positionToLight);
              lightDir = normalize(positionToLight);
              attenuation = 1.0 / (lights[i].constantAttenuation +
                                   lights[i].linearAttenuation * distance +
                                   lights[i].quadraticAttenuation * distance * distance);
              if (lights[i].spotCutoff <= 90.0)
                 {
                 float clampedCos = max(0.0, dot(-lightDir, normalize(lights[i].spotDirection)));
                 if (clampedCos < cos(radians(lights[i].spotCutoff)))
                    {
                    attenuation = 0.0;
                    }
                 else
                    {
                    attenuation = attenuation * pow(clampedCos, lights[i].spotExponent);
                    }
                 }
              }
              vec3 diffuseReflection = attenuation *
	                               vec3(lights[i].diffuse) * vec3(frontMaterial.diffuse) *
	                               max(0.0, dot(normalDir, lightDir));

              vec3 specularReflection;
              if (dot(N, lightDir) < 0.0) // light source on the wrong side?
	         {
	         specularReflection = vec3(0.0, 0.0, 0.0); // no specular reflection
	         }
              else // light source on the right side
	         {
	         specularReflection = attenuation * vec3(lights[i].specular) * vec3(frontMaterial.specular) *
	                              pow(max(0.0, dot(reflect(-lightDir, normalDir), viewDir)), frontMaterial.shininess);
	         }

           totalLighting = totalLighting + diffuseReflection + specularReflection;
           }
	vec3 t = normalize(v);
	if( isEqual(t,N)) {
		color = vec4(1.0,0.0,0.0,1.0);
	}else {
		color  = vec4(totalLighting, 1.0);
	}
}