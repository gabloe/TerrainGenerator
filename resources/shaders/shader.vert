#version 150

in vec3 v_Position;
in vec3 v_Normal;

uniform mat4 projection,translate;
uniform vec3 camera;

out vec3 color;

void main(void) {
	vec3 V = (translate * vec4(v_Position,1.0)).xyz;
	vec3 N = vec4(normalize(v_Normal),1.0).xyz;

	vec3 L = normalize(camera);
	vec3 E = normalize(-V);
	vec3 R = normalize(reflect(-L,N));

	float intensity = clamp(dot(L, N), 0.0, 1.0);

	vec3 ambient	= vec3(0.62,0.32,0.17); // vec4(0.5, 0.0, 0.0, 1.0);
	vec3 diffuse	= clamp(vec3(0.5, 0.5, 0.5) * max(dot(N,L), 0.0), 0.0, 1.0);
	vec3 specular	= clamp(vec3(0.0, 0.0, 0.20) * pow(max(dot(R,E),0.0), 0.3 ), 0.0, 1.0);

	color = intensity * (ambient+ diffuse + specular);
	gl_Position = projection * vec4(V,1.0);

}
