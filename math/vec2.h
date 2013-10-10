
#ifndef _VEC2_

#define _VEC2_

#include <ostream>

class vec2 {

private:
	float v[2];
public:
	vec2( float x, float y ) {
		v[0] = x;
		v[1] = y;
	}
	
	inline float getX() {
		return this->v[0];
	}
	
	inline float getY() {
		return this->v[1];
	}

	vec2 operator+(vec2 rhs) {
		return vec2( 
			getX() + rhs.getX() , 
			getY() + rhs.getY() 
		);
	}

	float operator*(vec2& rhs) {
		return 	( 
			getX() * rhs.getX() + 
			getY() * rhs.getY() 
		);
	
	}

	float sqrt_magintude() {
		return 	(
			( getX() * getX() ) + 
			( getY() * getY() )
		);
	}

	vec2 clone() {
		return vec2( 
			getX() , 
			getY()
		);
	}

};

std::ostream& operator<<( std::ostream& os , vec2 &obj) {
		os << "(" << 
		obj.getX() << "," << 
		obj.getY() << ")";
		return os;
}


#endif