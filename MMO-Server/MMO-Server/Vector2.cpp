#include "stdafx.h"
#include "Vector2.h"
#include <math.h>

Vector2::Vector2(float _x, float _y)
{
	x = _x;
	y = _y;
}

Vector2::Vector2()
{
	x = 0;
	y = 0;
}


Vector2::~Vector2(void)
{
}


float Vector2::getDistance(Vector2 p2)
{
	return sqrt((x-p2.x)*(x-p2.x)+(y-p2.y)*(y-p2.y));
}

Vector2 Vector2::operator-(Vector2 p2)
{
	Vector2 diff;
	diff.x = x-p2.x;
	diff.y = y-p2.y;
	return diff;
}

Vector2 Vector2::operator+(Vector2 p2)
{
	Vector2 diff;
	diff.x = x+p2.x;
	diff.y = y+p2.y;
	return diff;
}

Vector2 Vector2::operator*(float val)
{
	Vector2 diff;
	diff.x=x*val;
	diff.y=y*val;
	return diff;
}