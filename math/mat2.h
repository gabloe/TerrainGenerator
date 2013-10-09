

#ifndef _MAT2_

#include "vec2.h"

class mat2 {
private:
	float m[4];
public:

	mat2(vec2 c1 , vec2 c2 ) {
		m[0] = c1.v[0];
		m[1] = c1.v[1];
		
		m[2] = c2.v[0];
		m[3] = c2.v[1];
	}
	
	mat2(float m00 , float m01 , float m10 , float m11 ) {
		m[0] = m00;
		m[1] = m01;
		m[2] = m10;
		m[3] = m11;
	}

	// Add two matricies
	mat2 operator+( mat2 rhs ) {
		return mat2( 
			m[0] + rhs.m[0] , 
			m[1] + rhs.m[1] , 
			m[2] + rhs.m[2] , 
			m[3] + rhs.m[3] 
		);
	}
	
	// Multiply two matrices together
	mat2 operator*( mat2 rhs ) {
		return mat2( 
			m[0] * rhs.m[0] + m[1] * rhs.m[2] ,
			m[0] * rhs.m[1] + m[1] * rhs.m[3] ,
			m[2] * rhs.m[0] + m[3] * rhs.m[2] ,
			m[2] * rhs.m[1] + m[3] * rhs.m[3]
		);
	}
}

#define _MAT2_

#endif