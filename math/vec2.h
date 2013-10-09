
#ifndef _VEC2_

#define _VEC2_

#include <ostream>

class vec2 {
private:
	float v[2];
public:
	vec2( float v1, float v2 ) {
		v[0] = v1;
		v[1] = v2;
	}
	
	float getA() {
		return this->v[0];
	}
	
	float getB() {
		return this->v[1];
	}

	vec2 operator+(vec2 rhs) {
		return vec2( v[0] + rhs.v[0] , v[1] + rhs.v[1]);
	}

	float operator*(vec2 rhs) {
		return ( v[0] * rhs.v[0] + v[1] * rhs.v[1] );
	
	}

	float sqrt_magintude() {
		return ( getA() * getA()  ) + ( getB() * getB() );
	}

	vec2 clone() {
		return new vec2( v[0] , v[1] );
	}

};

std::ostream& operator<<( std::ostream& os , vec2 &obj) {
		os << "(" << obj.getA() << "," << obj.getB() << ")";
		return os;
}


#endif