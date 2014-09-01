#version 150

in vec3 v_Position;
in vec3 v_Normal;

uniform mat4 projection,translate;
uniform vec3 camera;

out vec3 n_cs;
out vec3 p_ws;
out vec3 ed_cs;
out vec3 ld_cs;

void main(void) {

	vec3 p_ws = v_Position;
	vec3 vp_cs = (translate * vec4(p_ws,1.0)).xyz;
	vec3 ed_cs = -vp_cs;

	vec3 lp_cs = (translate * vec4(camera,1.0)).xyz;
	vec3 ld_cs = lp_cs + ed_cs;

	n_cs = (translate * vec4(v_Normal,0)).xyz;
	
	gl_Position = projection * translate * vec4(v_Position,1.0);

}
