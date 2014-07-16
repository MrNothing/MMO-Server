#include "stdafx.h"
#include "Vector3.h"
#include <math.h>

Vector3::Vector3(float _x, float _y, float _z)
{
	x = _x;
	y = _y;
	z = _z;
}

Vector3::Vector3(void)
{
	x = 0;
	y = 0;
	z = 0;
}


Vector3::~Vector3(void)
{
}


float Vector3::getDistance(Vector3 p2)
{
	return sqrt((x-p2.x)*(x-p2.x)+(y-p2.y)*(y-p2.y)+(z-p2.z)*(z-p2.z));
}

Vector3 Vector3::operator-(Vector3 p2)
{
	Vector3 diff;
	diff.x = x-p2.x;
	diff.y = y-p2.y;
	diff.z = z-p2.z;
	return diff;
}

Vector3 Vector3::operator+(Vector3 p2)
{
	Vector3 diff;
	diff.x = x+p2.x;
	diff.y = y+p2.y;
	diff.z = z+p2.z;
	return diff;
}

Vector3 Vector3::operator*(float val)
{
	Vector3 diff;
	diff.x=x*val;
	diff.y=y*val;
	diff.z=z*val;
	return diff;
}

Vector3 Vector3::operator/(float val)
{
	Vector3 diff;
	diff.x=x/val;
	diff.y=y/val;
	diff.z=z/val;
	return diff;
}

Vector3 Vector3::operator*(Vector3 p2)
{
	Vector3 diff;
	diff.x = y*p2.z-z*p2.y;
	diff.y = z*p2.x-x*p2.z;
	diff.z = x*p2.y-y*p2.x;
	return diff;
}

float Vector3::magnitude()
{
	return abs(x)+abs(y)+abs(z);
}

float Vector3::sqrMagnitude()
{
	return sqrt(x*x+y*y+z*z);
}

std::string Vector3::toString()
{
	return "{x:"+std::to_string(x)+", y:"+std::to_string(y)+", z:"+std::to_string(z)+"}";
}

Vector3 Vector3::flatten(float step)
{
	return Vector3(floor(x/step)*step, floor(y/step)*step, floor(z/step)*step);
}

Vector3 Vector3::normalized()
{
	float avgLength = sqrMagnitude();
	return (*this)/avgLength;
}