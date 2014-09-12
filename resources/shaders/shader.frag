#version 150

out vec4 colorOut;

const vec3 lightSrc = vec3(0.0,1000000.0,0.0);
const vec4 ambient = vec4(0.2,0.2,0.2,1.0);
const vec4 diffuse = vec4(1.0,1.0,1.0,1.0);
const vec4 specular = vec4(0.1,0.1,0.1,1.0);

const float shininess = 0.2;

in vec3 N;
in vec3 v;

uniform vec3 p1,p2,p3;

out vec4 color;

bool isEqual(in vec3 a, in vec3 b) {
	if( abs(a.x - b.x) > 0.00001 ) return false;
	return abs(a.z - b.z) < 0.00001;
}


void main(void) {
	vec3 L = normalize(lightSrc - v);   
	vec3 E = normalize(-v); // we are in Eye Coordinates, so EyePos is (0,0,0)  
	vec3 R = normalize(-reflect(L,N));  
 
	//calculate Ambient Term:  
	vec4 Iamb = ambient;    

	//calculate Diffuse Term:  
	vec4 Idiff = diffuse * max(dot(N,L), 0.0);
	Idiff = clamp(Idiff, 0.0, 1.0);     
   
	// calculate Specular Term:
	vec4 Ispec = specular * pow(max(dot(R,E),0.0),0.3 * shininess);
	Ispec = clamp(Ispec, 0.0, 1.0); 

	vec3 t = normalize(v);
	// write Total Color:  
	if( isEqual(N,t)) {
		color = vec4(1.0, 0.0 ,0.0, 1.0);
	}else {
		color = max(ambient, clamp(Iamb + Idiff + Ispec, 0.0, 1.0)); 
	}
}
