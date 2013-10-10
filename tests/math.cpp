
#include <iostream>
#include <stdlib.h>

using namespace std;

#include "../math/vec2.h"
#include "../math/vec3.h"
#include "../math/vec4.h"
#include "../math/mat2.h"
#include "../math/mat3.h"
#include "../math/mat4.h"

int main() {

	vec2 v1(1.0f , 2.0f);
	vec3 v2( 1.0 , 2.0 , 3.0 );
	vec4 v3( 1.0 , 2.0 , 3.0 , 4.0 );
	
	mat2 m1(1.0f , 2.0f , 3.0f , 4.0f);
	mat3 m2( 1.0 , 2.0 , 3.0 , 4.0 , 5.0 , 6.0 , 7.0 , 8.0 , 9.0 );
	mat4 m3( 1.0 , 2.0 , 3.0 , 4.0, 5.0 , 6.0 , 7.0 , 8.0, 9.0 , 10.0 , 11.0 , 12.0, 13.0 , 14.0 , 15.0 , 16.0 );
	
	cout << v1 << endl;
	cout << v2 << endl;
	cout << v3 << endl;
	
	cout << m1 << endl;
	cout << m2 << endl;
	cout << m3 << endl;
	
	float r1 = v1 * v1;
	float r2 = v2 * v2;
	float r3 = v3 * v3;
	cout << r1 << endl;
	cout << r2 << endl;
	cout << r3 << endl;
	
	m1 = m1 * m1;
	m2 = m2 * m2;
	m3 = m3 * m3;
	cout << m1 << endl;
	cout << m2 << endl;
	cout << m3 << endl;
	
	return EXIT_SUCCESS;
}