#include "vec3.h"

#include <cmath>

Vec3::Vec3(void) {
	v[0] = v[1] = v[2] = 0.f;
}

Vec3::Vec3(float x, float y, float z) {
	v[0] = x;
	v[1] = y;
	v[2] = z;
}

Vec3::Vec3(const float* a) {
	v[0] = a[0];
	v[1] = a[1];
	v[2] = a[2];
}

float Vec3::getX() const {
	return this->v[0];
}

float Vec3::getY() const {
	return this->v[1];
}

float Vec3::getZ() const {
	return this->v[2];
}

Vec3 Vec3::operator+(const Vec3 &rhs) const {
	return Vec3(
		getX() + rhs.getX(),
		getY() + rhs.getY(),
		getZ() + rhs.getZ()
		);
}

Vec3 Vec3::operator-(const Vec3 &rhs) const {
	return Vec3(
		getX() - rhs.getX(),
		getY() - rhs.getY(),
		getZ() - rhs.getZ()
		);
}

float Vec3::operator*(const Vec3 &rhs) const {
	return (
		getX() * rhs.getX() + getY() * rhs.getY() + getZ() * rhs.getZ()
		);
}

bool Vec3::isZero() const {
	bool res = true;
	for (int i = 0; i < 3; ++i) res &= (abs(v[i]) < 0.00001);
	return res;
}

void Vec3::normalize() {
	float inv = 0.0;
	for (int i = 0; i < 3; ++i) inv += (v[i] * v[i]);
	scale(sqrt(1.0f / inv));
}

void Vec3::scale(float s) {
	for (int i = 0; i < 3; ++i) v[i] *= s;
}

Vec3 Vec3::clone() const {
	return Vec3(getX(), getY(), getZ());
}

Vec3 Vec3::cross(const Vec3 &b) const {
	return Vec3(
		getY() * b.getZ() - getZ()*b.getY(),
		getZ() * b.getX() - getX()*b.getZ(),
		getX() * b.getY() - getY()*b.getX()
		);
}

const float *Vec3::getData() const { return v; }

std::ostream& operator<<(std::ostream& os, Vec3 &obj) {
	os << "(" <<
		obj.getX() << "," <<
		obj.getY() << "," <<
		obj.getZ() << ")";
	return os;
}