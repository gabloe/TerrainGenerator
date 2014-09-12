#version 150

in vec3 v_Position;
in vec3 v_Normal;

uniform mat4 projection,translate;
uniform vec3 camera;


out vec3 v;
out vec3 N;
out mat4 normalMatrix;

bool isEqual(in vec3 a, in vec3 b) {
	if( abs(a.x - b.x) > 0.00001 ) return false;
	return abs(a.z - b.z) < 0.00001;
}

void main(void) {
	
	normalMatrix = transpose(inverse(translate));

	v = vec3(translate * vec4(v_Position,1.0));
	N = normalize(normalMatrix * vec4(v_Normal,0.0)).xyz;

	gl_Position = projection  * translate * vec4(v_Position,1.0);

	/*
	// Hope this works, can I compare vecs?
	if( isEqual(p1,v_Position) || isEqual(p2,v_Position) || isEqual(p3,v_Position) ) {
		color = vec4(1.0,0.0,0.0,1.0);
	}
	*/
}
