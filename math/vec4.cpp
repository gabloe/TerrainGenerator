#include "vec4.h"

Vec4::Vec4() {
	v[0] = v[1] = v[2] = v[3] = 0;
}

Vec4::Vec4(float x, float y, float z, float w) {
	v[0] = x;
	v[1] = y;
	v[2] = z;
	v[3] = w;
}

float Vec4::getX() const {
	return this->v[0];
}

float Vec4::getY() const {
	return this->v[1];
}

float Vec4::getZ() const {
	return this->v[2];
}

float Vec4::getW() const {
	return this->v[3];
}


Vec4 Vec4::operator+(Vec4 rhs) const {
	return Vec4(
		getX() + rhs.getX(),
		getY() + rhs.getY(),
		getZ() + rhs.getZ(),
		getW() + rhs.getW()
		);
}

float Vec4::operator*(Vec4 rhs) const {
	return (
		getX() * rhs.getX() +
		getY() * rhs.getY() +
		getZ() * rhs.getZ() +
		getW() * rhs.getW()
		);
}

Vec4 Vec4::clone() const {
	return Vec4(getX(), getY(), getZ(), getW());
}

std::ostream& operator<<(std::ostream& os, Vec4 &obj) {
	os << "(" <<
		obj.getX() << "," <<
		obj.getY() << "," <<
		obj.getZ() << "," <<
		obj.getW() << ")";
	return os;
}