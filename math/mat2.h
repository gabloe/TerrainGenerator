#pragma once

#include "vec2.h"

class MATH_API Mat2 {
private:
	float m[4];
public:
	Mat2(Vec2 c1, Vec2 c2);
	
	Mat2(float m00, float m01, float m10, float m11);

	// Add two matricies
	Mat2 operator+(Mat2 rhs);
	
	// Multiply two matrices together
	Mat2 operator*(Mat2 rhs);

	friend std::ostream& operator<<(std::ostream& os, Mat2 &obj);
};

std::ostream& operator<<(std::ostream& os, Mat2 &obj);
