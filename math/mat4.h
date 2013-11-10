#pragma once

#include "vec4.h"

class MATH_API Mat4 {
private:
	float m[16];
public:
	Mat4(Vec4 r1, Vec4 r2, Vec4 r3, Vec4 r4);
	
	Mat4(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33
		);

	// Add two matricies
	Mat4 operator+(Mat4 rhs);
	
	// Multiply two matrices together
	Mat4 operator*(Mat4 rhs);

	friend std::ostream& operator<<(std::ostream& os, Mat4 &obj);
};

std::ostream& operator<<(std::ostream& os, Mat4 &obj);