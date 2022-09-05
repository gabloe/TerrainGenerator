#version 330 core

in vec3 v_Position;
in vec3 v_Normal;

uniform mat4 projection,translate;
uniform vec3 camera;


out vec3 v;
out vec3 N;
out mat4 normalMatrix;
out vec4 vertexColor;

vec4 green = vec4(0.07, 0.29, 0.13, 1.0);
vec4 brown = vec4(0.51, 0.4, 0.22, 1.0);
vec4 black = vec4(0.0, 0.0, 0.0, 1.0);

bool isEqual(in vec3 a, in vec3 b) {
	if( abs(a.x - b.x) > 0.00001 ) return false;
	return abs(a.z - b.z) < 0.00001;
}

void main(void) {
	
	normalMatrix = transpose(inverse(translate));

	if (v_Position.y < 0.001)
	{
		vertexColor = brown;
	}
	else if (v_Position.y < 0.02)
	{
		vertexColor = green;
	}
	else
	{
		vertexColor = black;
	}

	v = vec3(translate * vec4(v_Position,1.0));
	N = normalize(normalMatrix * vec4(v_Normal,0.0)).xyz;

	gl_Position = projection * translate * vec4(v_Position,1.0);
}
