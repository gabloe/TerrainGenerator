#pragma once

#include "Def.h"
#include <ostream>

class MATH_API Vec2{

private:
	float v[2];
public:
	Vec2();
	Vec2(float x, float y);

	float getX() const;
	float getY() const;

	Vec2 operator+(const Vec2 &rhs) const;
	Vec2 operator-(const Vec2 &rhs) const;
	float operator*(const Vec2& rhs) const; // Dot Product

	Vec2 &operator-=(const Vec2 &rhs);
	Vec2 &operator+=(const Vec2 &rhs);

	float getMagnitude() const;

	Vec2 clone() const;

};

std::ostream& operator<<(std::ostream& os, Vec2 &obj);