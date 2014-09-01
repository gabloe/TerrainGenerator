#version 150

in vec3 v_Position;
in vec3 v_Normal;

uniform mat4 projection,translate;
uniform vec3 camera;

out vec4 color;

void main(void) {
	vec4 V = translate * vec4(v_Position,1.0);
	vec4 N = vec4(normalize(v_Normal),1.0);

	gl_Position = projection * V;

	vec4 L = normalize(vec4(camera,1.0) - V);
	vec4 E = normalize(-V);
	vec4 R = normalize(reflect(-L,N));

	vec4 ambient	= vec4(0.62,0.32,0.17,1.0); // vec4(0.5, 0.0, 0.0, 1.0);
	vec4 diffuse	= clamp(vec4(0.62, 0.32, 0.17, 1.0) * max(dot(N,L), 0.0), 0.0, 1.0);
	vec4 specular	= clamp(vec4(0.62, 0.32, 0.17, 1.0) * pow(max(dot(R,E),0.0), 0.3 ), 0.0, 1.0);

	color = ambient + diffuse + specular;
}
