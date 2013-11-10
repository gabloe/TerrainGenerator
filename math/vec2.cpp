
#include "vec2.h"

Vec2::Vec2() {
	v[0] = v[1] = 0;
}

Vec2::Vec2(float x, float y) {
	v[0] = x;
	v[1] = y;
}

float Vec2::getX() {
	return v[0];
}

float Vec2::getY() {
	return v[1];
}


Vec2 Vec2::operator+(Vec2 rhs) {
	return Vec2(
		getX() + rhs.getX(),
		getY() + rhs.getY()
		);
}


float Vec2::operator*(Vec2& rhs) {
	return 	(
		getX() * rhs.getX() +
		getY() * rhs.getY()
		);
}

Vec2 Vec2::clone() {
	return Vec2(
		getX(),
		getY()
		);
}


std::ostream& operator<<(std::ostream& os, Vec2 &obj) {
	os << "(" <<
		obj.getX() << "," <<
		obj.getY() << ")";
	return os;
}
