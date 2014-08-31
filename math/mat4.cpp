#include "mat4.h"

#include <cmath>

Mat4::Mat4() {
	for (int i = 0; i < 16; ++i) m[i] = 0;
	m[0] = 1.0f;
	m[5] = 1.0f;
	m[10] = 1.0f;
	m[15] = 1.0f;
}

Mat4::Mat4(const Mat4 &other) {
	std::memcpy(m, other.m, sizeof(float)* 16);
}

Mat4::Mat4(const Vec4 &r1, const Vec4 &r2, const Vec4 &r3, const Vec4 &r4) {
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

// Add two matricies
Mat4 Mat4::operator+(const Mat4 &rhs) {
	Mat4 ret;
	for (int i = 0; i < 16; ++i) {
		ret.m[i] = m[i] + rhs.m[i];
	}
	return ret;
}

// Multiply two matrices together
Mat4 Mat4::operator*(const Mat4 &rhs) {
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

void Mat4::rotateX(float angle) {
	Mat4 t = Mat4::RotateX(angle);
	*this = *this * t;
}

void Mat4::rotateY(float angle) {
	Mat4 t = Mat4::RotateY(angle);
	*this = *this * t;
}

void Mat4::rotateZ(float angle) {
	Mat4 t = Mat4::RotateZ(angle);
	*this = *this * t;
}

Vec3 normalize(const Vec3& other) {
	float div = other * other;
	return Vec3(other.getX() / div, other.getY() / div, other.getZ() / div);
}

// Static Class Functions
Mat4 Mat4::Perspective(float fov, float aspect, float near, float far) {
	Mat4 ret;
	float range = tan((fov * 0.0174532925f * 0.5f)) * near;
	float left = -range * aspect;
	float right = range * aspect;
	float bottom = -range;
	float top = range;

	ret.m[0] = (2 * near) / (right - left);
	ret.m[5] = (2 * near) / (top - bottom);
	ret.m[10] = -(far + near) / (far - near);
	ret.m[11] = -1;
	ret.m[14] = -(2 * far * near) / (far - near);
	ret.m[15] = 0;

	return ret;
}

Mat4 Mat4::LookAt(const Vec3 &eye, const Vec3 &center, const Vec3 &up) {
	Mat4 ret;

	Vec3 f = normalize(center - eye);
	Vec3 u = normalize(up);
	Vec3 s = normalize(f.cross(u));
	u = s.cross(f);

	ret.m[0] = s.getX();
	ret.m[4] = s.getY();
	ret.m[8] = s.getZ();

	ret.m[1] = u.getX();
	ret.m[5] = u.getY();
	ret.m[9] = u.getZ();

	ret.m[2] = -f.getX();
	ret.m[6] = -f.getY();
	ret.m[10] = -f.getZ();

	ret.m[12] = -(s * eye);
	ret.m[13] = -(u * eye);
	ret.m[14] = -(f * eye);

	ret.m[15] = 0;

	return ret;
}

Mat4 Mat4::RotateX(float x) {
	Mat4 ret;
	ret.m[5]  = cos(x);
	ret.m[6]  = -sin(x);
	ret.m[9]  = sin(x);
	ret.m[10] = cos(x);
	return ret;
}

Mat4 Mat4::RotateY(float x) {
	Mat4 ret;
	ret.m[0] = cos(x);
	ret.m[2] = sin(x);
	ret.m[8] = -sin(x);
	ret.m[10] = cos(x);
	return ret;
}

Mat4 Mat4::RotateZ(float x) {
	Mat4 ret;
	ret.m[0] = cos(x);
	ret.m[1] = -sin(x);
	ret.m[4] = sin(x);
	ret.m[5] = cos(x);
	return ret;
}

// Print
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

