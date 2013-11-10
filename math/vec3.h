#pragma once

#include "Def.h"

#include <ostream>
class MATH_API Vec3 {

private:
	float v[3];
public:
	Vec3(void);
	Vec3(float x, float y, float z);

	float getX();
	float getY();
	float getZ();

	Vec3 operator+(Vec3 rhs);
	float operator*(Vec3 rhs);

	Vec3 clone();

};

std::ostream& operator<<(std::ostream& os, Vec3 &obj);
