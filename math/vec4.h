
#ifndef _VEC4_

#define _VEC4_

#include <ostream>

class vec4 {
private:
	float v[4];
public:
	vec4( float x, float y , float z , float w) {
		v[0] = x;
		v[1] = y;
		v[2] = z;
		v[3] = w;
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
	
	inline float getW() {
		return this->v[3];
	}

	vec4 operator+(vec4 rhs) {
		return vec4( 
			getX() + rhs.getX() , 
			getY() + rhs.getY() ,
			getZ() + rhs.getZ() ,
			getW() + rhs.getW()
		);
	}

	float operator*(vec4 rhs) {
		return ( 
			getX() * rhs.getX() + 
			getY() * rhs.getY() + 
			getZ() * rhs.getZ() + 
			getW() * rhs.getW()
		);
	}

	float sqrt_magintude() {
		return 	(
					( getX() * getX() ) + 
					( getY() * getY() ) + 
					( getZ() * getZ() ) + 
					( getW() * getW() )
				);
	}

	vec4 clone() {
		return vec4( getX() , getY() , getZ() , getW() );
	}

};

std::ostream& operator<<( std::ostream& os , vec4 &obj) {
		os 	<< "(" << 
		obj.getX() << "," << 
		obj.getY() << "," << 
		obj.getZ() << "," << 
		obj.getW() << ")";
		return os;
}


#endif