#include "stdafx.h"
#include "Vector3i.h"
#include <math.h>

Vector3i::Vector3i(int _x, int _y, int _z)
{
	x = _x;
	y = _y;
	z = _z;
}

Vector3i::Vector3i(void)
{
	x = 0;
	y = 0;
	z = 0;
}


Vector3i::~Vector3i(void)
{
}


float Vector3i::getDistance(Vector3i p2)
{
	return sqrt((x-p2.x)*(x-p2.x)+(y-p2.y)*(y-p2.y)+(z-p2.z)*(z-p2.z));
}

Vector3i Vector3i::operator-(Vector3i p2)
{
	Vector3i diff;
	diff.x = x-p2.x;
	diff.y = y-p2.y;
	diff.z = z-p2.z;
	return diff;
}

Vector3i Vector3i::operator+(Vector3i p2)
{
	Vector3i diff;
	diff.x = x+p2.x;
	diff.y = y+p2.y;
	diff.z = z+p2.z;
	return diff;
}

Vector3i Vector3i::operator*(int val)
{
	Vector3i diff;
	diff.x=x*val;
	diff.y=y*val;
	diff.z=z*val;
	return diff;
}

Vector3i Vector3i::operator/(int val)
{
	Vector3i diff;
	diff.x=x/val;
	diff.y=y/val;
	diff.z=z/val;
	return diff;
}

Vector3i Vector3i::operator*(Vector3i p2)
{
	Vector3i diff;
	diff.x = y*p2.z-z*p2.y;
	diff.y = z*p2.x-x*p2.z;
	diff.z = x*p2.y-y*p2.x;
	return diff;
}

float Vector3i::magnitude()
{
	return abs(x)+abs(y)+abs(z);
}

float Vector3i::sqrMagnitude()
{
	return sqrt(x*x+y*y+z*z);
}

std::string Vector3i::toString()
{
	return "{x:"+std::to_string(x)+", y:"+std::to_string(y)+", z:"+std::to_string(z)+"}";
}

Vector3i Vector3i::flatten(float step)
{
	return Vector3i(floor(x/step)*step, floor(y/step)*step, floor(z/step)*step);
}

Vector3i Vector3i::normalized()
{
	float avgLength = sqrMagnitude();
	return (*this)/avgLength;
}