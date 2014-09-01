#version 150

in vec3 v_Position;
in vec3 v_Normal;

uniform mat4 projection,translate;
uniform vec3 camera;

out vec3 color;

void main(void) {
	vec3 V = (vec4(v_Position,1.0)).xyz;
	vec3 N = (vec4(normalize(v_Normal),1.0)).xyz;

	gl_Position = projection * translate * vec4(v_Position,1.0);

	vec3 LightPosition = vec3(0.0,-1000.0,0.0);
        vec3 cameraToVertex = V - camera;
        float distance = length(cameraToVertex);
	vec3 L = normalize(LightPosition - camera);
	vec3 E = normalize(-V);
	vec3 R = normalize(reflect(-L,N));

	float attenuation = clamp(100.0 / distance, 0.0, 1.0);

	vec3 ambient	= vec3(0.2,0.1,0.1); // vec4(0.5, 0.0, 0.0, 1.0);
	vec3 diffuse	= clamp(vec3(0.5, 0.5, 0.5) * max(dot(-N,L), 0.0), 0.0, 1.0);
	vec3 specular	= clamp(vec3(0.62, 0.32, 0.17) * pow(max(dot(R,E),0.0), 0.5 ), 0.0, 1.0);

	color = ambient + diffuse + specular;
        color = attenuation * color;
}
