#pragma once

#include "Def.h"

#include <ostream>

class MATH_API Vec4 {
private:
	float v[4];
public:
	Vec4();
	Vec4(float x, float y, float z, float w);

	float getX() const;
	float getY() const;
	float getZ() const;
	float getW() const;

	Vec4 operator+(Vec4 rhs) const;
	float operator*(Vec4 rhs) const;

	Vec4 clone() const;

	MATH_API friend std::ostream& operator<<(std::ostream& os, Vec4 &obj);
};
