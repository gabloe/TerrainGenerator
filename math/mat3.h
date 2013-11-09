#pragma once

#include "vec3.h"

class MATH_API Mat3 {
private:
	float m[9];
public:
	Mat3(Vec3 r1, Vec3 r2, Vec3 r3);
	
	Mat3(
		float m00, float m01, float m02,
		float m10, float m11, float m12,
		float m20, float m21, float m22
		);

	// Add two matricies
	Mat3 operator+(Mat3 rhs);
	
	// Multiply two matrices together
	Mat3 operator*(Mat3 rhs);

	friend std::ostream& operator<<(std::ostream& os, Mat3 &obj);
};

std::ostream& operator<<(std::ostream& os, Mat3 &obj);