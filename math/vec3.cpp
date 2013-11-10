#include "vec3.h"

Vec3::Vec3(void) {
	v[0] = v[1] = v[2] = 0.f;
}

Vec3::Vec3(float x, float y, float z) {
	v[0] = x;
	v[1] = y;
	v[2] = z;
}

float Vec3::getX() {
	return this->v[0];
}

float Vec3::getY() {
	return this->v[1];
}

float Vec3::getZ() {
	return this->v[2];
}

Vec3 Vec3::operator+(Vec3 rhs) {
	return Vec3(
		getX() + rhs.getX(),
		getY() + rhs.getY(),
		getZ() + rhs.getZ()
		);
}

float Vec3::operator*(Vec3 rhs) {
	return (
		getX() * rhs.getX() + getY() * rhs.getY() + getZ() * rhs.getZ()
		);
}

Vec3 Vec3::clone() {
	return Vec3(getX(), getY(), getZ());
}


std::ostream& operator<<(std::ostream& os, Vec3 &obj) {
	os << "(" <<
		obj.getX() << "," <<
		obj.getY() << "," <<
		obj.getZ() << ")";
	return os;
}