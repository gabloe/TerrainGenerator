#include "mat3.h"

Mat3::Mat3(Vec3 r1, Vec3 r2, Vec3 r3) {
	m[0] = r1.getX();
	m[1] = r1.getY();
	m[2] = r1.getY();

	m[3] = r2.getX();
	m[4] = r2.getY();
	m[5] = r2.getY();

	m[6] = r3.getX();
	m[7] = r3.getY();
	m[8] = r3.getY();

}

Mat3::Mat3(
	float m00, float m01, float m02,
	float m10, float m11, float m12,
	float m20, float m21, float m22
	) {
	m[0] = m00;
	m[1] = m01;
	m[2] = m02;

	m[3] = m10;
	m[4] = m11;
	m[5] = m12;

	m[6] = m20;
	m[7] = m21;
	m[8] = m22;

}

// Add two matricies
Mat3 Mat3::operator+(Mat3 rhs) {
	return Mat3(
		m[0] + rhs.m[0],
		m[1] + rhs.m[1],
		m[2] + rhs.m[2],
		m[3] + rhs.m[3],
		m[4] + rhs.m[4],
		m[5] + rhs.m[5],
		m[6] + rhs.m[6],
		m[7] + rhs.m[7],
		m[8] + rhs.m[8]
		);
}

// Multiply two matrices together
Mat3 Mat3::operator*(Mat3 rhs) {
	return Mat3(
		m[0] * rhs.m[0] + m[1] * rhs.m[3] + m[2] * rhs.m[6],
		m[0] * rhs.m[1] + m[1] * rhs.m[4] + m[2] * rhs.m[7],
		m[0] * rhs.m[2] + m[1] * rhs.m[5] + m[2] * rhs.m[8],

		m[3] * rhs.m[0] + m[4] * rhs.m[3] + m[5] * rhs.m[6],
		m[3] * rhs.m[1] + m[4] * rhs.m[4] + m[5] * rhs.m[7],
		m[3] * rhs.m[2] + m[4] * rhs.m[5] + m[5] * rhs.m[8],

		m[6] * rhs.m[0] + m[7] * rhs.m[3] + m[8] * rhs.m[6],
		m[6] * rhs.m[1] + m[7] * rhs.m[4] + m[8] * rhs.m[7],
		m[6] * rhs.m[2] + m[7] * rhs.m[5] + m[8] * rhs.m[8]
		);
}

std::ostream& operator<<(std::ostream& os, Mat3 &obj) {
	os << "(" <<
		obj.m[0] << " " <<
		obj.m[1] << " " <<
		obj.m[2] << "\n" <<
		obj.m[3] << " " <<
		obj.m[4] << " " <<
		obj.m[5] << "\n" <<
		obj.m[6] << " " <<
		obj.m[7] << " " <<
		obj.m[8] << ")";
	return os;
}