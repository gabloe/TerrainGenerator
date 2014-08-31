#version 150

in vec3 norm;

out vec4 colorOut;

void main(void) {
<<<<<<< HEAD
    colorOut= vec4(abs(norm.x),abs(norm.y),abs(norm.z), 1.0);
=======
    float derp= dot( vec4(norm,1.0), vec4(1.0,0.0,0.0,1.0));
    colorOut= derp * vec4(1.0, 0.0, 0.0, 1.0);
>>>>>>> 5f3c72c57ff028d4046971984f905e4958a791df
}
