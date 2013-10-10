
#ifndef _VEC3_

#define _VEC3_

#include <ostream>

class vec3 {
private:
	float v[3];
public:
	vec3( float x, float y , float z ) {
		v[0] = x;
		v[1] = y;
		v[2] = z;
	}
	
	inline float getX() {
		return this->v[0];
	}
	
	inline float getY() {
		return this->v[1];
	}
	
	inline float getZ() {
		return this->v[2];
	}

	vec3 operator+(vec3 rhs) {
		return vec3( 
			getX() + rhs.getX() , 
			getY() + rhs.getY() ,
			getZ() + rhs.getZ()
		);
	}

	float operator*(vec3 rhs) {
		return ( 
			getX() * rhs.getX() + getY() * rhs.getY() + getZ() * rhs.getZ()
		);
	}

	float sqrt_magintude() {
		return ( getX() * getX()  ) + ( getY() * getY() ) + ( getZ() * getZ() );
	}

	vec3 clone() {
		return vec3( getX() , getY() , getZ() );
	}

};

std::ostream& operator<<( std::ostream& os , vec3 &obj) {
		os << "(" << 
		obj.getX() << "," << 
		obj.getY() << "," << 
		obj.getZ() << ")";
		return os;
}

#endif