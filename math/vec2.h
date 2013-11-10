#pragma once

#include "Def.h"
#include <ostream>

class MATH_API Vec2{

private:
	float v[2];
public:
	Vec2();
	Vec2(float x, float y);

	float getX();
	float getY();

	Vec2 operator+(Vec2 rhs);
	float operator*(Vec2& rhs); // Dot Product

	Vec2 clone();

};

std::ostream& operator<<(std::ostream& os, Vec2 &obj);