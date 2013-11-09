#include "mat2.h"

Mat2::Mat2(Vec2 c1, Vec2 c2) {
	m[0] = c1.getX();
	m[1] = c1.getY();

	m[2] = c2.getX();
	m[3] = c2.getY();
}

Mat2::Mat2(float m00, float m01, float m10, float m11) {
	m[0] = m00;
	m[1] = m01;
	m[2] = m10;
	m[3] = m11;
}

// Add two matricies
Mat2 Mat2::operator+(Mat2 rhs) {
	return Mat2(
		m[0] + rhs.m[0],
		m[1] + rhs.m[1],
		m[2] + rhs.m[2],
		m[3] + rhs.m[3]
		);
}

// Multiply two matrices together
Mat2 Mat2::operator*(Mat2 rhs) {
	return Mat2(
		m[0] * rhs.m[0] + m[1] * rhs.m[2],
		m[0] * rhs.m[1] + m[1] * rhs.m[3],
		m[2] * rhs.m[0] + m[3] * rhs.m[2],
		m[2] * rhs.m[1] + m[3] * rhs.m[3]
		);
}

std::ostream& operator<<(std::ostream& os, Mat2 &obj) {
	os << "(" << obj.m[0] << " " << obj.m[1] << " , " << obj.m[2] << " " << obj.m[3] << ")";
	return os;
}
