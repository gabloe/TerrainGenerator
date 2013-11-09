#pragma once

#include "Def.h"

#include <ostream>

class MATH_API Vec4 {
private:
	float v[4];
public:
	Vec4();
	Vec4(float x, float y, float z, float w);

	float getX();
	float getY();
	float getZ();
	float getW();

	Vec4 operator+(Vec4 rhs);
	float operator*(Vec4 rhs);

	Vec4 clone();
};
std::ostream& operator<<(std::ostream& os, Vec4 &obj);
