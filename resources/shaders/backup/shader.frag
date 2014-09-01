#version 150

uniform vec3 camera;

in vec3 n_cs;
in vec3 p_ws;
in vec3 ed_cs;
in vec3 ld_cs;

out vec3 color;

void main(void) {

	vec3 LightColor = vec3(1,1,1);
	float LightPower = 50.0;
	
	vec3 MaterialDiffuseColor = vec3(0.4,0.4,1.0);
	vec3 MaterialAmbientColor = vec3(0.1,0.1,0.1) * MaterialDiffuseColor;
	vec3 MaterialSpecularColor = vec3(0.3,0.3,0.3);
	
	float distance = length( camera - p_ws );
	vec3 n = normalize( n_cs );
	vec3 l = normalize( ld_cs );
	
	float cosTheta = clamp( dot( n,l ), 0,1 );
	
	vec3 E = normalize(ed_cs);
	vec3 R = reflect(-l,n);
	
	float cosAlpha = clamp( dot( E,R ), 0,1 );
	color = 
		MaterialAmbientColor +
		MaterialDiffuseColor * LightColor * LightPower * cosTheta / (distance*distance) +
		MaterialSpecularColor * LightColor * LightPower * pow(cosAlpha,5) / (distance*distance);
}
