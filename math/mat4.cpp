#include "mat4.h"

Mat4::Mat4() {
	for (int i = 0; i < 16; ++i) m[i] = 0;
}

Mat4::Mat4(Mat4 &other) {
	for (int i = 0; i < 16; ++i) m[i] = other.m[i];
}

Mat4::Mat4(Vec4 r1, Vec4 r2, Vec4 r3, Vec4 r4) {
	m[0] = r1.getX();
	m[1] = r1.getY();
	m[2] = r1.getZ();
	m[3] = r1.getW();

	m[4] = r2.getX();
	m[5] = r2.getY();
	m[6] = r2.getZ();
	m[7] = r2.getW();

	m[8] = r3.getX();
	m[9] = r3.getY();
	m[10] = r3.getZ();
	m[11] = r3.getW();

	m[12] = r4.getX();
	m[13] = r4.getY();
	m[14] = r4.getZ();
	m[15] = r4.getW();

}

Mat4::Mat4(
	float m00, float m01, float m02, float m03,
	float m10, float m11, float m12, float m13,
	float m20, float m21, float m22, float m23,
	float m30, float m31, float m32, float m33
	) {
	m[0] = m00;
	m[1] = m01;
	m[2] = m02;
	m[3] = m03;

	m[4] = m10;
	m[5] = m11;
	m[6] = m12;
	m[7] = m13;

	m[8] = m20;
	m[9] = m21;
	m[10] = m22;
	m[11] = m23;

	m[12] = m30;
	m[13] = m31;
	m[14] = m32;
	m[15] = m33;
}

/*
Mat4::Mat4(Mat4& other) {
	memcpy(this->m, other.m, sizeof(float)* 16);
}
*/
// Add two matricies
Mat4 Mat4::operator+(Mat4 &rhs) {
	return Mat4(m[0] + rhs.m[0], m[1] + rhs.m[1],
		m[2] + rhs.m[2],
		m[3] + rhs.m[3],
		m[4] + rhs.m[4],
		m[5] + rhs.m[5],
		m[6] + rhs.m[6],
		m[7] + rhs.m[7],
		m[8] + rhs.m[8],
		m[9] + rhs.m[9],
		m[10] + rhs.m[10],
		m[11] + rhs.m[11],
		m[12] + rhs.m[12],
		m[13] + rhs.m[13],
		m[14] + rhs.m[14],
		m[15] + rhs.m[15]
		);
}

// Multiply two matrices together
Mat4 Mat4::operator*(Mat4 &rhs) {
	return Mat4(
		m[0] * rhs.m[0] + m[1] * rhs.m[4] + m[2] * rhs.m[8] + m[3] * rhs.m[12],
		m[0] * rhs.m[1] + m[1] * rhs.m[5] + m[2] * rhs.m[9] + m[3] * rhs.m[13],
		m[0] * rhs.m[2] + m[1] * rhs.m[6] + m[2] * rhs.m[10] + m[3] * rhs.m[14],
		m[0] * rhs.m[3] + m[1] * rhs.m[7] + m[2] * rhs.m[11] + m[3] * rhs.m[15],

		m[4] * rhs.m[0] + m[5] * rhs.m[4] + m[6] * rhs.m[8] + m[7] * rhs.m[12],
		m[4] * rhs.m[1] + m[5] * rhs.m[5] + m[6] * rhs.m[9] + m[7] * rhs.m[13],
		m[4] * rhs.m[2] + m[5] * rhs.m[6] + m[6] * rhs.m[10] + m[7] * rhs.m[14],
		m[4] * rhs.m[3] + m[5] * rhs.m[7] + m[6] * rhs.m[11] + m[7] * rhs.m[15],

		m[8] * rhs.m[0] + m[9] * rhs.m[4] + m[10] * rhs.m[8] + m[11] * rhs.m[12],
		m[8] * rhs.m[1] + m[9] * rhs.m[5] + m[10] * rhs.m[9] + m[11] * rhs.m[13],
		m[8] * rhs.m[2] + m[9] * rhs.m[6] + m[10] * rhs.m[10] + m[11] * rhs.m[14],
		m[8] * rhs.m[3] + m[9] * rhs.m[7] + m[10] * rhs.m[11] + m[11] * rhs.m[15],

		m[12] * rhs.m[0] + m[13] * rhs.m[4] + m[14] * rhs.m[8] + m[15] * rhs.m[12],
		m[12] * rhs.m[1] + m[13] * rhs.m[5] + m[14] * rhs.m[9] + m[15] * rhs.m[13],
		m[12] * rhs.m[2] + m[13] * rhs.m[6] + m[14] * rhs.m[10] + m[15] * rhs.m[14],
		m[12] * rhs.m[3] + m[13] * rhs.m[7] + m[14] * rhs.m[11] + m[15] * rhs.m[15]
		);
}



std::ostream& operator<<(std::ostream& os, Mat4& obj) {
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			os << obj.m[i * 4 + j] << " ";
		}
		os << std::endl;
	}
	return os;
}

